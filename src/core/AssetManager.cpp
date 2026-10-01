#include "AssetManager.h"
#include <QUrl>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

AssetManager::AssetManager(QObject *parent) : QObject(parent) {
    createSampleBroadcastAssets();
}

QString AssetManager::formatBytes(qint64 bytes) const {
    if (bytes < 1024) return QString("%1 B").arg(bytes);
    if (bytes < 1024 * 1024) return QString("%1 KB").arg(QString::number(bytes / 1024.0, 'f', 1));
    return QString("%1 MB").arg(QString::number(bytes / (1024.0 * 1024.0), 'f', 1));
}

void AssetManager::createSampleBroadcastAssets() {
    m_assets.clear();

    BroadcastAsset a1;
    a1.id = "asset_logo_svg";
    a1.name = "Makasna Logo Master (SVG)";
    a1.filePath = ":/logo.svg";
    a1.fileUrl = "qrc:/logo.svg";
    a1.mediaType = "svg";
    a1.typeLabel = "[VECTOR SVG]";
    a1.fileSize = 12400;
    a1.fileSizeFormatted = "12.1 KB";
    a1.width = 1920;
    a1.height = 1080;
    a1.isTransparent = true;
    a1.createdAt = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm");
    addAssetInternal(a1);

    BroadcastAsset a2;
    a2.id = "asset_banner_svg";
    a2.name = "Makasna Studio Banner (SVG)";
    a2.filePath = ":/logo-banner.svg";
    a2.fileUrl = "qrc:/logo-banner.svg";
    a2.mediaType = "svg";
    a2.typeLabel = "[VECTOR SVG]";
    a2.fileSize = 18600;
    a2.fileSizeFormatted = "18.2 KB";
    a2.width = 1920;
    a2.height = 360;
    a2.isTransparent = true;
    a2.createdAt = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm");
    addAssetInternal(a2);

    BroadcastAsset a3;
    a3.id = "asset_ae_lowerthird";
    a3.name = "After Effects Lower Third Glitch (Lottie JSON)";
    a3.filePath = "templates/assets/ae_lowerthird_glitch.json";
    a3.fileUrl = "qrc:/qt/qml/Makasna/BroadcastCG/templates/assets/ae_lowerthird_glitch.json";
    a3.mediaType = "lottie";
    a3.typeLabel = "[AE LOTTIE]";
    a3.fileSize = 145000;
    a3.fileSizeFormatted = "141.6 KB";
    a3.width = 1920;
    a3.height = 250;
    a3.durationMs = 4000;
    a3.isTransparent = true;
    a3.createdAt = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm");
    addAssetInternal(a3);

    BroadcastAsset a4;
    a4.id = "asset_video_stinger";
    a4.name = "Alpha Stinger Transition (ProRes 4444 / WebM)";
    a4.filePath = "templates/assets/stinger_transition.webm";
    a4.fileUrl = "";
    a4.mediaType = "video";
    a4.typeLabel = "[VIDEO ALPHA]";
    a4.fileSize = 4850000;
    a4.fileSizeFormatted = "4.6 MB";
    a4.width = 1920;
    a4.height = 1080;
    a4.durationMs = 2500;
    a4.isTransparent = true;
    a4.createdAt = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm");
    addAssetInternal(a4);

    emit assetsChanged();
}

void AssetManager::addAssetInternal(const BroadcastAsset &asset) {
    m_assets.append(asset);
}

QVariantList AssetManager::assets() const {
    QVariantList list;
    for (const auto &a : m_assets) {
        list.append(a.toMap());
    }
    return list;
}

QVariantList AssetManager::filteredAssets() const {
    if (m_filterType == "all" || m_filterType.isEmpty()) {
        return assets();
    }

    QVariantList list;
    for (const auto &a : m_assets) {
        if (a.mediaType == m_filterType) {
            list.append(a.toMap());
        }
    }
    return list;
}

void AssetManager::setFilterType(const QString &filter) {
    if (m_filterType != filter) {
        m_filterType = filter;
        emit filterTypeChanged();
        emit assetsChanged();
    }
}

QString AssetManager::detectAssetType(const QString &fileName) const {
    QString ext = QFileInfo(fileName).suffix().toLower();
    if (ext == "json") return "lottie";
    if (ext == "mp4" || ext == "mov" || ext == "webm" || ext == "avi" || ext == "mkv") return "video";
    if (ext == "svg") return "svg";
    if (ext == "png" || ext == "jpg" || ext == "jpeg" || ext == "webp" || ext == "bmp" || ext == "tiff") return "image";
    if (ext == "wav" || ext == "mp3" || ext == "aac" || ext == "flac" || ext == "ogg") return "audio";
    return "media";
}

bool AssetManager::importAsset(const QString &filePathOrUrl) {
    QString cleanPath = filePathOrUrl;
    if (cleanPath.startsWith("file:///")) {
        cleanPath = QUrl(cleanPath).toLocalFile();
    }

    QFileInfo fi(cleanPath);
    if (!fi.exists() && !cleanPath.startsWith("qrc:")) {
        qWarning() << "[AssetManager] File does not exist:" << cleanPath;
        return false;
    }

    QString ext = fi.suffix().toLower();
    QString type = detectAssetType(cleanPath);
    QString label = "[MEDIA]";

    if (type == "lottie") {
        label = "[AE LOTTIE]";
    } else if (type == "video") {
        label = (ext == "mov" || ext == "webm") ? "[VIDEO ALPHA]" : "[VIDEO]";
    } else if (type == "svg") {
        label = "[VECTOR SVG]";
    } else if (type == "image") {
        label = (ext == "png" || ext == "webp") ? "[PNG 32-BIT]" : "[RASTER IMAGE]";
    } else if (type == "audio") {
        label = "[AUDIO]";
    }

    BroadcastAsset asset;
    asset.id = QString("asset_%1_%2").arg(ext).arg(QDateTime::currentMSecsSinceEpoch());
    asset.name = fi.completeBaseName();
    asset.filePath = cleanPath;
    asset.fileUrl = cleanPath.startsWith("qrc:") ? cleanPath : QUrl::fromLocalFile(cleanPath).toString();
    asset.mediaType = type;
    asset.typeLabel = label;
    asset.fileSize = fi.size();
    asset.fileSizeFormatted = formatBytes(fi.size());
    asset.width = 1920;
    asset.height = 1080;
    asset.isTransparent = (type == "lottie" || type == "svg" || ext == "png" || ext == "mov" || ext == "webm");
    asset.createdAt = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm");

    m_assets.insert(0, asset);
    emit assetsChanged();
    emit assetImported(asset.name, asset.typeLabel);

    qInfo() << "[AssetManager] Successfully imported broadcast asset:"
            << asset.name << "Type:" << asset.typeLabel << "Path:" << cleanPath;
    return true;
}

int AssetManager::importAssets(const QStringList &filePathsOrUrls) {
    int count = 0;
    for (const auto &p : filePathsOrUrls) {
        if (importAsset(p)) {
            count++;
        }
    }
    return count;
}

bool AssetManager::removeAsset(int index) {
    if (index >= 0 && index < m_assets.size()) {
        QString removedName = m_assets[index].name;
        m_assets.removeAt(index);
        emit assetsChanged();
        qInfo() << "[AssetManager] Removed asset:" << removedName;
        return true;
    }
    return false;
}

void AssetManager::clearAssets() {
    m_assets.clear();
    emit assetsChanged();
}

QVariantMap AssetManager::getAsset(int index) const {
    if (index >= 0 && index < m_assets.size()) {
        return m_assets[index].toMap();
    }
    return QVariantMap();
}
