#pragma once

#include <QObject>
#include <QString>
#include <QVariantList>
#include <QVariantMap>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

class ProjectModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString activeTemplateId READ activeTemplateId WRITE setActiveTemplateId NOTIFY activeTemplateIdChanged)
    Q_PROPERTY(QString selectedLayerId READ selectedLayerId WRITE setSelectedLayerId NOTIFY selectedLayerIdChanged)
    Q_PROPERTY(double currentTime READ currentTime WRITE setCurrentTime NOTIFY currentTimeChanged)
    Q_PROPERTY(bool isPlaying READ isPlaying WRITE setIsPlaying NOTIFY isPlayingChanged)
    Q_PROPERTY(double duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(QVariantList templates READ templates NOTIFY templatesChanged)
    Q_PROPERTY(QVariantList rundown READ rundown NOTIFY rundownChanged)
    Q_PROPERTY(QVariantList datasets READ datasets NOTIFY datasetsChanged)

public:
    explicit ProjectModel(QObject *parent = nullptr);

    QString activeTemplateId() const { return m_activeTemplateId; }
    void setActiveTemplateId(const QString &id);

    QString selectedLayerId() const { return m_selectedLayerId; }
    void setSelectedLayerId(const QString &id);

    double currentTime() const { return m_currentTime; }
    void setCurrentTime(double time);

    bool isPlaying() const { return m_isPlaying; }
    void setIsPlaying(bool playing);

    double duration() const;

    QVariantList templates() const { return m_templates; }
    QVariantList rundown() const { return m_rundown; }
    QVariantList datasets() const { return m_datasets; }

    Q_INVOKABLE QVariantMap getActiveTemplate() const;
    Q_INVOKABLE QVariantMap getSelectedLayer() const;

    // Direct Layer Free Drag & Manipulation with Motion Path Sync
    Q_INVOKABLE void moveLayer(const QString &templateId, const QString &layerId, double deltaX, double deltaY);
    Q_INVOKABLE void updateLayerTransform(const QString &templateId, const QString &layerId, double x, double y, double width, double height);
    Q_INVOKABLE void updateLayerContent(const QString &templateId, const QString &layerId, const QString &text);
    Q_INVOKABLE void updateLayerBinding(const QString &templateId, const QString &layerId, const QString &columnName);

    // Keyframe Management
    Q_INVOKABLE void updateKeyframeTime(const QString &templateId, const QString &layerId, const QString &kfId, double newTime);
    Q_INVOKABLE void addKeyframeAtCurrentTime(const QString &templateId, const QString &layerId);

    // Dataset Management
    Q_INVOKABLE void addDataset(const QString &name, const QStringList &headers, const QVariantList &rows);
    Q_INVOKABLE void importCsvData(const QString &datasetId, const QString &csvContent);
    Q_INVOKABLE QVariantList getDatasetRows(const QString &datasetId) const;

signals:
    void activeTemplateIdChanged();
    void selectedLayerIdChanged();
    void currentTimeChanged();
    void isPlayingChanged();
    void durationChanged();
    void templatesChanged();
    void rundownChanged();
    void datasetsChanged();
    void layerTransformed(const QString &layerId, double x, double y);

private:
    void initDefaultProject();

    QString m_activeTemplateId;
    QString m_selectedLayerId;
    double m_currentTime = 0.0;
    bool m_isPlaying = false;
    QVariantList m_templates;
    QVariantList m_rundown;
    QVariantList m_datasets;
};
