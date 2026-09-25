#pragma once

#include <QObject>
#include <QString>
#include <QVariantList>
#include <QVariantMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFileSystemWatcher>
#include <QTimer>
#include <QDateTime>

class ProjectModel;

class SmartDataManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isSyncing READ isSyncing NOTIFY isSyncingChanged)
    Q_PROPERTY(QString lastSyncTime READ lastSyncTime NOTIFY lastSyncTimeChanged)
    Q_PROPERTY(QString activeSourceType READ activeSourceType NOTIFY activeSourceTypeChanged)
    Q_PROPERTY(QString activeSourcePath READ activeSourcePath NOTIFY activeSourcePathChanged)
    Q_PROPERTY(int pollInterval READ pollInterval WRITE setPollInterval NOTIFY pollIntervalChanged)
    Q_PROPERTY(QVariantList availableDatasets READ availableDatasets NOTIFY availableDatasetsChanged)

public:
    explicit SmartDataManager(ProjectModel *projectModel, QObject *parent = nullptr);

    bool isSyncing() const { return m_isSyncing; }
    QString lastSyncTime() const { return m_lastSyncTime; }
    QString activeSourceType() const { return m_activeSourceType; }
    QString activeSourcePath() const { return m_activeSourcePath; }
    int pollInterval() const { return m_pollInterval; }
    void setPollInterval(int seconds);

    QVariantList availableDatasets() const;

    // Methods accessible from QML
    Q_INVOKABLE void connectLocalFile(const QString &filePath, const QString &format); // "csv", "json", "excel"
    Q_INVOKABLE void connectRemoteJson(const QString &url, int refreshIntervalSec = 3);
    Q_INVOKABLE void connectRemoteXml(const QString &url, int refreshIntervalSec = 5);
    Q_INVOKABLE void connectGoogleSheet(const QString &sheetUrlOrId, int refreshIntervalSec = 5);
    Q_INVOKABLE void manualRefresh();
    Q_INVOKABLE void stopAutoSync();

signals:
    void isSyncingChanged();
    void lastSyncTimeChanged();
    void activeSourceTypeChanged();
    void activeSourcePathChanged();
    void pollIntervalChanged();
    void availableDatasetsChanged();
    void dataUpdated(const QString &sourceName, int rowCount);
    void syncError(const QString &errorMessage);

private slots:
    void onLocalFileChanged(const QString &path);
    void onPollTimeout();
    void onNetworkReply(QNetworkReply *reply);

private:
    void parseAndApplyCsv(const QString &data, const QString &sourceName);
    void parseAndApplyJson(const QByteArray &data, const QString &sourceName);
    void parseAndApplyXml(const QByteArray &data, const QString &sourceName);
    QString extractGoogleSheetCsvUrl(const QString &input);

    ProjectModel *m_projectModel = nullptr;
    QNetworkAccessManager *m_networkManager = nullptr;
    QFileSystemWatcher *m_fileWatcher = nullptr;
    QTimer *m_pollTimer = nullptr;

    bool m_isSyncing = false;
    QString m_lastSyncTime = "-";
    QString m_activeSourceType = "None";
    QString m_activeSourcePath = "";
    int m_pollInterval = 5; // seconds
};
