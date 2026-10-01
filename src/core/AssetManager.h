#pragma once

#include <QObject>
#include <QString>
#include <QVariantList>
#include <QVariantMap>
#include <QFileInfo>
#include <QDateTime>

struct BroadcastAsset {
    QString id;
    QString name;
    QString filePath;
    QString fileUrl;
    QString mediaType;   // "lottie", "video", "svg", "image", "audio"
    QString typeLabel;   // "[AE LOTTIE]", "[VIDEO ALPHA]", "[VECTOR SVG]", "[PNG 32-BIT]", "[AUDIO]"
    qint64 fileSize = 0;
    QString fileSizeFormatted;
    int width = 1920;
    int height = 1080;
    qint64 durationMs = 0;
    bool isTransparent = true;
    QString createdAt;

    QVariantMap toMap() const {
        QVariantMap map;
        map["id"] = id;
        map["name"] = name;
        map["filePath"] = filePath;
        map["fileUrl"] = fileUrl;
        map["mediaType"] = mediaType;
        map["typeLabel"] = typeLabel;
        map["fileSize"] = fileSize;
        map["fileSizeFormatted"] = fileSizeFormatted;
        map["width"] = width;
        map["height"] = height;
        map["durationMs"] = durationMs;
        map["isTransparent"] = isTransparent;
        map["createdAt"] = createdAt;
        return map;
    }
};

class AssetManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList assets READ assets NOTIFY assetsChanged)
    Q_PROPERTY(int totalAssets READ totalAssets NOTIFY assetsChanged)
    Q_PROPERTY(QString filterType READ filterType WRITE setFilterType NOTIFY filterTypeChanged)
    Q_PROPERTY(QVariantList filteredAssets READ filteredAssets NOTIFY assetsChanged)

public:
    explicit AssetManager(QObject *parent = nullptr);

    QVariantList assets() const;
    int totalAssets() const { return m_assets.size(); }
    QString filterType() const { return m_filterType; }
    void setFilterType(const QString &filter);
    QVariantList filteredAssets() const;

    Q_INVOKABLE bool importAsset(const QString &filePathOrUrl);
    Q_INVOKABLE int importAssets(const QStringList &filePathsOrUrls);
    Q_INVOKABLE bool removeAsset(int index);
    Q_INVOKABLE void clearAssets();
    Q_INVOKABLE QVariantMap getAsset(int index) const;
    Q_INVOKABLE void createSampleBroadcastAssets();
    Q_INVOKABLE QString detectAssetType(const QString &fileName) const;

signals:
    void assetsChanged();
    void filterTypeChanged();
    void assetImported(const QString &name, const QString &typeLabel);

private:
    QString formatBytes(qint64 bytes) const;
    void addAssetInternal(const BroadcastAsset &asset);

    QList<BroadcastAsset> m_assets;
    QString m_filterType = "all"; // "all", "lottie", "video", "image", "audio"
};
