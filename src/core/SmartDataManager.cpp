#include "SmartDataManager.h"
#include "ProjectModel.h"
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QXmlStreamReader>
#include <QRegularExpression>
#include <QUrl>
#include <QDebug>

SmartDataManager::SmartDataManager(ProjectModel *projectModel, QObject *parent)
    : QObject(parent), m_projectModel(projectModel) {
    m_networkManager = new QNetworkAccessManager(this);
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &SmartDataManager::onNetworkReply);

    m_fileWatcher = new QFileSystemWatcher(this);
    connect(m_fileWatcher, &QFileSystemWatcher::fileChanged, this, &SmartDataManager::onLocalFileChanged);

    m_pollTimer = new QTimer(this);
    connect(m_pollTimer, &QTimer::timeout, this, &SmartDataManager::onPollTimeout);
}

void SmartDataManager::setPollInterval(int seconds) {
    if (seconds < 1) seconds = 1;
    if (m_pollInterval != seconds) {
        m_pollInterval = seconds;
        if (m_pollTimer->isActive()) {
            m_pollTimer->start(m_pollInterval * 1000);
        }
        emit pollIntervalChanged();
    }
}

QVariantList SmartDataManager::availableDatasets() const {
    if (m_projectModel) {
        return m_projectModel->datasets();
    }
    return QVariantList();
}

void SmartDataManager::connectLocalFile(const QString &filePath, const QString &format) {
    QString cleanPath = filePath;
    if (cleanPath.startsWith("file:///")) {
        cleanPath = QUrl(cleanPath).toLocalFile();
    }

    QFileInfo fi(cleanPath);
    if (!fi.exists()) {
        emit syncError("File tidak ditemukan: " + cleanPath);
        return;
    }

    // Clear previous watched files
    QStringList watched = m_fileWatcher->files();
    if (!watched.isEmpty()) {
        m_fileWatcher->removePaths(watched);
    }

    m_fileWatcher->addPath(cleanPath);
    m_activeSourceType = "Local File (" + format.toUpper() + ")";
    m_activeSourcePath = cleanPath;
    m_isSyncing = true;

    emit activeSourceTypeChanged();
    emit activeSourcePathChanged();
    emit isSyncingChanged();

    // Initial read
    onLocalFileChanged(cleanPath);
}

void SmartDataManager::connectRemoteJson(const QString &url, int refreshIntervalSec) {
    stopAutoSync();
    m_activeSourceType = "Remote JSON API";
    m_activeSourcePath = url.trimmed();
    setPollInterval(refreshIntervalSec);
    m_isSyncing = true;

    emit activeSourceTypeChanged();
    emit activeSourcePathChanged();
    emit isSyncingChanged();

    // Trigger immediate fetch & start timer
    manualRefresh();
    m_pollTimer->start(m_pollInterval * 1000);
}

void SmartDataManager::connectRemoteXml(const QString &url, int refreshIntervalSec) {
    stopAutoSync();
    m_activeSourceType = "Remote XML / RSS";
    m_activeSourcePath = url.trimmed();
    setPollInterval(refreshIntervalSec);
    m_isSyncing = true;

    emit activeSourceTypeChanged();
    emit activeSourcePathChanged();
    emit isSyncingChanged();

    // Trigger immediate fetch & start timer
    manualRefresh();
    m_pollTimer->start(m_pollInterval * 1000);
}

QString SmartDataManager::extractGoogleSheetCsvUrl(const QString &input) {
    QString trimmed = input.trimmed();
    // Matches spreadsheet ID in https://docs.google.com/spreadsheets/d/{ID}/...
    QRegularExpression re(R"(/spreadsheets/d/([a-zA-Z0-9-_]+))");
    QRegularExpressionMatch match = re.match(trimmed);

    QString sheetId;
    if (match.hasMatch()) {
        sheetId = match.captured(1);
    } else {
        // Assume input is raw Sheet ID
        sheetId = trimmed;
    }

    // Check for gid (specific sheet tab)
    QString gid = "0";
    QRegularExpression gidRe(R"([?&#]gid=([0-9]+))");
    QRegularExpressionMatch gidMatch = gidRe.match(trimmed);
    if (gidMatch.hasMatch()) {
        gid = gidMatch.captured(1);
    }

    return QString("https://docs.google.com/spreadsheets/d/%1/export?format=csv&gid=%2").arg(sheetId, gid);
}

void SmartDataManager::connectGoogleSheet(const QString &sheetUrlOrId, int refreshIntervalSec) {
    stopAutoSync();
    QString csvUrl = extractGoogleSheetCsvUrl(sheetUrlOrId);

    m_activeSourceType = "Google Sheets (Live)";
    m_activeSourcePath = csvUrl;
    setPollInterval(refreshIntervalSec);
    m_isSyncing = true;

    emit activeSourceTypeChanged();
    emit activeSourcePathChanged();
    emit isSyncingChanged();

    // Trigger immediate fetch & start timer
    manualRefresh();
    m_pollTimer->start(m_pollInterval * 1000);
}

void SmartDataManager::manualRefresh() {
    if (m_activeSourcePath.isEmpty()) return;

    if (m_activeSourceType.startsWith("Local File")) {
        onLocalFileChanged(m_activeSourcePath);
    } else if (m_activeSourceType.contains("Remote") || m_activeSourceType.contains("Google Sheets")) {
        QUrl targetUrl(m_activeSourcePath);
        QNetworkRequest request(targetUrl);
        request.setHeader(QNetworkRequest::UserAgentHeader, "Makasna-Broadcast-CG/1.0");
        request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
        m_networkManager->get(request);
    }
}

void SmartDataManager::stopAutoSync() {
    m_pollTimer->stop();
    QStringList watched = m_fileWatcher->files();
    if (!watched.isEmpty()) {
        m_fileWatcher->removePaths(watched);
    }
    m_isSyncing = false;
    emit isSyncingChanged();
}

void SmartDataManager::onLocalFileChanged(const QString &path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit syncError("Gagal membuka file: " + file.errorString());
        return;
    }

    QByteArray rawData = file.readAll();
    file.close();

    // If editor rewrote the file, re-watch it
    if (!m_fileWatcher->files().contains(path)) {
        m_fileWatcher->addPath(path);
    }

    QFileInfo fi(path);
    QString baseName = fi.baseName();
    QString suffix = fi.suffix().toLower();

    if (suffix == "json") {
        parseAndApplyJson(rawData, baseName);
    } else {
        // CSV or exported Excel table
        parseAndApplyCsv(QString::fromUtf8(rawData), baseName);
    }

    m_lastSyncTime = QDateTime::currentDateTime().toString("hh:mm:ss");
    emit lastSyncTimeChanged();
}

void SmartDataManager::onPollTimeout() {
    manualRefresh();
}

void SmartDataManager::onNetworkReply(QNetworkReply *reply) {
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        emit syncError("HTTP Request Error: " + reply->errorString());
        return;
    }

    QByteArray data = reply->readAll();
    QString srcName = "OnlineDataset";

    if (m_activeSourceType.contains("Google Sheets")) {
        parseAndApplyCsv(QString::fromUtf8(data), "GoogleSheet_Data");
    } else if (m_activeSourceType.contains("JSON")) {
        parseAndApplyJson(data, "Remote_JSON");
    } else if (m_activeSourceType.contains("XML")) {
        parseAndApplyXml(data, "Remote_XML");
    }

    m_lastSyncTime = QDateTime::currentDateTime().toString("hh:mm:ss");
    emit lastSyncTimeChanged();
}

void SmartDataManager::parseAndApplyCsv(const QString &data, const QString &sourceName) {
    if (!m_projectModel) return;

    QStringList lines = data.split(QRegularExpression(R"(\r\n|\n|\r)"), Qt::SkipEmptyParts);
    if (lines.isEmpty()) return;

    // Detect delimiter (, or ;)
    QChar delim = ',';
    if (lines[0].count(';') > lines[0].count(',')) {
        delim = ';';
    }

    // Parse headers
    QStringList rawHeaders = lines[0].split(delim);
    QStringList headers;
    for (QString h : rawHeaders) {
        headers.append(h.trimmed().remove('\"'));
    }

    // Parse rows
    QVariantList rows;
    for (int i = 1; i < lines.size(); ++i) {
        QString line = lines[i].trimmed();
        if (line.isEmpty()) continue;

        QStringList cols = line.split(delim);
        QVariantMap rowMap;
        for (int c = 0; c < headers.size() && c < cols.size(); ++c) {
            rowMap[headers[c]] = cols[c].trimmed().remove('\"');
        }
        rows.append(rowMap);
    }

    // Inject into project model
    m_projectModel->addDataset(sourceName, headers, rows);
    emit availableDatasetsChanged();
    emit dataUpdated(sourceName, rows.size());

    qDebug() << "[SmartData] Loaded CSV dataset:" << sourceName << "with" << rows.size() << "rows";
}

void SmartDataManager::parseAndApplyJson(const QByteArray &data, const QString &sourceName) {
    if (!m_projectModel) return;

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        emit syncError("Format JSON tidak valid");
        return;
    }

    QJsonArray array;
    if (doc.isArray()) {
        array = doc.array();
    } else if (doc.isObject()) {
        QJsonObject obj = doc.object();
        // Look for common array keys like "data", "items", "rows", "results"
        if (obj.contains("data") && obj["data"].isArray()) {
            array = obj["data"].toArray();
        } else if (obj.contains("items") && obj["items"].isArray()) {
            array = obj["items"].toArray();
        } else if (obj.contains("rows") && obj["rows"].isArray()) {
            array = obj["rows"].toArray();
        } else {
            // Single object wrapped as 1 row
            array.append(obj);
        }
    }

    if (array.isEmpty()) return;

    // Extract headers from first item
    QStringList headers;
    if (array[0].isObject()) {
        headers = array[0].toObject().keys();
    }

    QVariantList rows;
    for (const QJsonValue &val : array) {
        if (val.isObject()) {
            rows.append(val.toObject().toVariantMap());
        }
    }

    m_projectModel->addDataset(sourceName, headers, rows);
    emit availableDatasetsChanged();
    emit dataUpdated(sourceName, rows.size());

    qDebug() << "[SmartData] Loaded JSON dataset:" << sourceName << "with" << rows.size() << "rows";
}

void SmartDataManager::parseAndApplyXml(const QByteArray &data, const QString &sourceName) {
    if (!m_projectModel) return;

    QXmlStreamReader xml(data);
    QStringList headers;
    QVariantList rows;

    QVariantMap currentRow;
    QString currentElement;
    bool inRecord = false;

    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();

        if (token == QXmlStreamReader::StartElement) {
            QString name = xml.name().toString();
            // RSS item or generic row/record/item
            if (name == "item" || name == "entry" || name == "row" || name == "record" || name == "player" || name == "match") {
                inRecord = true;
                currentRow.clear();
            } else if (inRecord) {
                currentElement = name;
                if (!headers.contains(name)) {
                    headers.append(name);
                }
            }
        } else if (token == QXmlStreamReader::Characters && inRecord && !currentElement.isEmpty()) {
            QString text = xml.text().toString().trimmed();
            if (!text.isEmpty()) {
                currentRow[currentElement] = text;
            }
        } else if (token == QXmlStreamReader::EndElement) {
            QString name = xml.name().toString();
            if (name == "item" || name == "entry" || name == "row" || name == "record" || name == "player" || name == "match") {
                if (!currentRow.isEmpty()) {
                    rows.append(currentRow);
                }
                inRecord = false;
            }
            currentElement.clear();
        }
    }

    if (xml.hasError()) {
        emit syncError("XML Parsing error: " + xml.errorString());
        return;
    }

    m_projectModel->addDataset(sourceName, headers, rows);
    emit availableDatasetsChanged();
    emit dataUpdated(sourceName, rows.size());

    qDebug() << "[SmartData] Loaded XML dataset:" << sourceName << "with" << rows.size() << "rows";
}
