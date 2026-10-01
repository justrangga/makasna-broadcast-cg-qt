#include "DeckLinkController.h"
#include <QDebug>

#ifdef _WIN32
#include <windows.h>
#include <objbase.h>
#include <oleauto.h>
#include <setupapi.h>

// Official Blackmagic DeckLink SDK COM GUIDs for Windows Desktop Video
// CLSID_CDeckLinkIterator = D9EDA3B3-2887-41FA-B724-017CF1EB1D37
static const CLSID BM_CLSID_CDeckLinkIterator = 
    {0xD9EDA3B3, 0x2887, 0x41FA, {0xB7, 0x24, 0x01, 0x7C, 0xF1, 0xEB, 0x1D, 0x37}};

// IID_IDeckLinkIterator = 74E936FC-CC28-4A67-81A0-1E94E52D4E69
static const IID BM_IID_IDeckLinkIterator = 
    {0x74E936FC, 0xCC28, 0x4A67, {0x81, 0xA0, 0x1E, 0x94, 0xE5, 0x2D, 0x4E, 0x69}};

// IID_IDeckLink = 62BFF75D-6569-4E55-8D4D-66AA03829ABC
static const IID BM_IID_IDeckLink = 
    {0x62BFF75D, 0x6569, 0x4E55, {0x8D, 0x4D, 0x66, 0xAA, 0x03, 0x82, 0x9A, 0xBC}};

class IDeckLink : public IUnknown {
public:
    virtual HRESULT STDMETHODCALLTYPE GetModelName(BSTR* modelName) = 0;
};

class IDeckLinkIterator : public IUnknown {
public:
    virtual HRESULT STDMETHODCALLTYPE Next(IDeckLink** deckLinkInstance) = 0;
};
#endif

DeckLinkController::DeckLinkController(QObject *parent) : QObject(parent) {
    m_frameTimer = new QTimer(this);
    m_frameTimer->setInterval(16); // ~60fps
    connect(m_frameTimer, &QTimer::timeout, this, &DeckLinkController::onFrameTick);

    m_scanTimer = new QTimer(this);
    m_scanTimer->setInterval(4000); // Check hardware topology every 4s
    connect(m_scanTimer, &QTimer::timeout, this, &DeckLinkController::onAutoScanTick);

    // Initial hardware discovery
    refreshDevices();

    m_scanTimer->start();
}

void DeckLinkController::refreshDevices() {
    QVariantList scanned = scanDevicesInternal();
    m_cachedDevices = scanned;

    m_hasHardwareDevices = false;
    for (const auto &dev : m_cachedDevices) {
        if (dev.toMap().value("isHardware").toBool()) {
            m_hasHardwareDevices = true;
            break;
        }
    }

    if (!m_cachedDevices.isEmpty()) {
        m_selectedDevice = m_cachedDevices[0].toMap().value("name").toString();
    } else {
        m_selectedDevice = "DeckLink SDI Auto Playout";
    }

    emit devicesChanged();
    emit selectedDeviceChanged();
}

void DeckLinkController::onAutoScanTick() {
    QVariantList scanned = scanDevicesInternal();
    if (scanned.size() != m_cachedDevices.size()) {
        qInfo() << "[DeckLink] Hardware topology change detected. Refreshing list...";
        refreshDevices();
    }
}

QVariantList DeckLinkController::getDevices() const {
    return m_cachedDevices;
}

QVariantList DeckLinkController::scanDevicesInternal() {
    QVariantList list;
    int index = 0;

#ifdef _WIN32
    // Strategy 1: Official Blackmagic Desktop Video COM API Discovery
    HRESULT hrInit = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    IDeckLinkIterator* deckLinkIterator = nullptr;
    HRESULT hr = CoCreateInstance(BM_CLSID_CDeckLinkIterator, NULL, CLSCTX_ALL, BM_IID_IDeckLinkIterator, (void**)&deckLinkIterator);

    if (SUCCEEDED(hr) && deckLinkIterator != nullptr) {
        IDeckLink* deckLink = nullptr;
        while (deckLinkIterator->Next(&deckLink) == S_OK) {
            BSTR modelName = NULL;
            deckLink->GetModelName(&modelName);

            QString qModelName = QString::fromWCharArray(modelName ? modelName : L"Blackmagic DeckLink");
            if (modelName) SysFreeString(modelName);

            QVariantMap dev;
            dev["index"] = index++;
            dev["name"] = QString("[HARDWARE] %1 (Sub-channel %2)").arg(qModelName).arg(index);
            dev["modelName"] = qModelName;
            dev["isHardware"] = true;
            dev["hasKeyer"] = true;
            dev["supports4K"] = qModelName.contains("4K") || qModelName.contains("8K") || qModelName.contains("12G");
            list.append(dev);

            deckLink->Release();
        }
        deckLinkIterator->Release();
    }

    // Strategy 2: Windows SetupAPI PnP PCIe/USB Device Enumeration (Vendor ID 0x1BDB)
    // Guarantees detection even if Desktop Video COM service is registering under alternate apartment
    HDEVINFO deviceInfoSet = SetupDiGetClassDevs(NULL, TEXT("PCI"), NULL, DIGCF_ALLCLASSES | DIGCF_PRESENT);
    if (deviceInfoSet != INVALID_HANDLE_VALUE) {
        SP_DEVINFO_DATA deviceInfoData;
        deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
        for (DWORD i = 0; SetupDiEnumDeviceInfo(deviceInfoSet, i, &deviceInfoData); ++i) {
            TCHAR hwId[512] = {0};
            TCHAR desc[512] = {0};
            if (SetupDiGetDeviceRegistryProperty(deviceInfoSet, &deviceInfoData, SPDRP_HARDWAREID, NULL, (PBYTE)hwId, sizeof(hwId), NULL)) {
                QString qHwId = QString::fromWCharArray(hwId).toUpper();
                // 1BDB is Blackmagic Design official PCI Vendor ID
                if (qHwId.contains("VEN_1BDB")) {
                    QString deviceName = "Blackmagic DeckLink PCIe";
                    if (SetupDiGetDeviceRegistryProperty(deviceInfoSet, &deviceInfoData, SPDRP_DEVICEDESC, NULL, (PBYTE)desc, sizeof(desc), NULL)) {
                        deviceName = QString::fromWCharArray(desc);
                    } else if (SetupDiGetDeviceRegistryProperty(deviceInfoSet, &deviceInfoData, SPDRP_FRIENDLYNAME, NULL, (PBYTE)desc, sizeof(desc), NULL)) {
                        deviceName = QString::fromWCharArray(desc);
                    }

                    // Avoid duplicate if already enumerated by COM
                    bool alreadyFound = false;
                    for (const auto &item : list) {
                        if (item.toMap().value("modelName").toString().compare(deviceName, Qt::CaseInsensitive) == 0) {
                            alreadyFound = true;
                            break;
                        }
                    }
                    if (!alreadyFound) {
                        QVariantMap dev;
                        dev["index"] = index++;
                        dev["name"] = QString("[HARDWARE] %1 (PCIe Direct)").arg(deviceName);
                        dev["modelName"] = deviceName;
                        dev["isHardware"] = true;
                        dev["hasKeyer"] = true;
                        dev["supports4K"] = deviceName.contains("4K") || deviceName.contains("8K") || deviceName.contains("12G");
                        list.append(dev);
                    }
                }
            }
        }
        SetupDiDestroyDeviceInfoList(deviceInfoSet);
    }
#endif

    // If actual Blackmagic PCIe/Thunderbolt hardware was detected on Windows, return it directly!
    if (!list.isEmpty()) {
        qInfo() << "[DeckLink] Successfully detected" << list.size() << "physical Blackmagic device(s).";
        return list;
    }

    // Default broadcast simulation profiles when no physical Blackmagic hardware is plugged in
    QVariantMap dev1;
    dev1["index"] = 0;
    dev1["name"] = "[SIMULASI] Blackmagic DeckLink Duo 2 (Channel 1 - SDI Fill & Key)";
    dev1["modelName"] = "DeckLink Duo 2";
    dev1["isHardware"] = false;
    dev1["hasKeyer"] = true;
    dev1["supports4K"] = false;
    list.append(dev1);

    QVariantMap dev2;
    dev2["index"] = 1;
    dev2["name"] = "[SIMULASI] Blackmagic DeckLink Duo 2 (Channel 2 - SDI Clean)";
    dev2["modelName"] = "DeckLink Duo 2";
    dev2["isHardware"] = false;
    dev2["hasKeyer"] = true;
    dev2["supports4K"] = false;
    list.append(dev2);

    QVariantMap dev3;
    dev3["index"] = 2;
    dev3["name"] = "[SIMULASI] Blackmagic DeckLink Quad 2 (SDI Playout 1-8)";
    dev3["modelName"] = "DeckLink Quad 2";
    dev3["isHardware"] = false;
    dev3["hasKeyer"] = true;
    dev3["supports4K"] = false;
    list.append(dev3);

    QVariantMap dev4;
    dev4["index"] = 3;
    dev4["name"] = "[SIMULASI] Blackmagic DeckLink 8K Pro (SDI 1-4 12G-SDI)";
    dev4["modelName"] = "DeckLink 8K Pro";
    dev4["isHardware"] = false;
    dev4["hasKeyer"] = true;
    dev4["supports4K"] = true;
    list.append(dev4);

    return list;
}

bool DeckLinkController::startPlayout(int deviceIndex, const QString &standard, const QString &keyer) {
    if (deviceIndex >= 0 && deviceIndex < m_cachedDevices.size()) {
        m_selectedDevice = m_cachedDevices[deviceIndex].toMap().value("name").toString();
    }
    m_videoStandard = standard;
    m_keyerMode = keyer;
    m_isStreaming = true;
    m_outputFrames = 0;
    m_droppedFrames = 0;
    m_genlockLocked = true;

    m_frameTimer->start();

    emit selectedDeviceChanged();
    emit videoStandardChanged();
    emit keyerModeChanged();
    emit isStreamingChanged();
    emit genlockLockedChanged();

    qInfo() << "[DeckLink] Playout started on device:" << m_selectedDevice
            << "Standard:" << m_videoStandard
            << "Keyer Mode:" << m_keyerMode;
    return true;
}

void DeckLinkController::stopPlayout() {
    m_isStreaming = false;
    m_frameTimer->stop();

    emit isStreamingChanged();
    qInfo() << "[DeckLink] Playout stopped.";
}

void DeckLinkController::onFrameTick() {
    if (!m_isStreaming) return;

    m_outputFrames++;

    if (m_outputFrames % 3600 == 0) {
        m_genlockLocked = true;
        emit genlockLockedChanged();
    }

    emit outputFramesChanged();
}
