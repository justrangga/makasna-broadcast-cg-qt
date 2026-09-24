#include "DeckLinkController.h"
#include <QDebug>

DeckLinkController::DeckLinkController(QObject *parent) : QObject(parent) {
    m_frameTimer = new QTimer(this);
    m_frameTimer->setInterval(16); // ~60fps
    connect(m_frameTimer, &QTimer::timeout, this, &DeckLinkController::onFrameTick);

    m_selectedDevice = "Blackmagic DeckLink Duo 2 (Channel 1 - SDI Fill & Key)";
}

QVariantList DeckLinkController::getDevices() const {
    QVariantList list;

    QVariantMap dev1;
    dev1["index"] = 0;
    dev1["name"] = "Blackmagic DeckLink Duo 2 (Channel 1 - SDI Fill & Key)";
    dev1["modelName"] = "DeckLink Duo 2";
    dev1["hasKeyer"] = true;
    dev1["supports4K"] = false;
    list.append(dev1);

    QVariantMap dev2;
    dev2["index"] = 1;
    dev2["name"] = "Blackmagic DeckLink Duo 2 (Channel 2 - SDI)";
    dev2["modelName"] = "DeckLink Duo 2";
    dev2["hasKeyer"] = true;
    dev2["supports4K"] = false;
    list.append(dev2);

    QVariantMap dev3;
    dev3["index"] = 2;
    dev3["name"] = "Blackmagic DeckLink Quad 2 (SDI 1-8)";
    dev3["modelName"] = "DeckLink Quad 2";
    dev3["hasKeyer"] = true;
    dev3["supports4K"] = false;
    list.append(dev3);

    QVariantMap dev4;
    dev4["index"] = 3;
    dev4["name"] = "Blackmagic DeckLink 8K Pro (SDI 1-4 12G-SDI)";
    dev4["modelName"] = "DeckLink 8K Pro";
    dev4["hasKeyer"] = true;
    dev4["supports4K"] = true;
    list.append(dev4);

    return list;
}

bool DeckLinkController::startPlayout(int deviceIndex, const QString &standard, const QString &keyer) {
    QVariantList devices = getDevices();
    if (deviceIndex >= 0 && deviceIndex < devices.size()) {
        m_selectedDevice = devices[deviceIndex].toMap().value("name").toString();
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

    qDebug() << "[DeckLink] Started Native SDI Playout on device:" << m_selectedDevice
             << "Standard:" << m_videoStandard << "Keyer:" << m_keyerMode;
    return true;
}

void DeckLinkController::stopPlayout() {
    if (m_isStreaming) {
        m_isStreaming = false;
        m_frameTimer->stop();
        emit isStreamingChanged();
        qDebug() << "[DeckLink] Stopped SDI Playout";
    }
}

void DeckLinkController::onFrameTick() {
    if (m_isStreaming) {
        m_outputFrames++;
        emit outputFramesChanged();
    }
}
