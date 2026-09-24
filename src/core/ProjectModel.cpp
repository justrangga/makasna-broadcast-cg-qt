#include "ProjectModel.h"
#include <QDateTime>
#include <QDebug>
#include <algorithm>

ProjectModel::ProjectModel(QObject *parent) : QObject(parent) {
    initDefaultProject();
}

void ProjectModel::setActiveTemplateId(const QString &id) {
    if (m_activeTemplateId != id) {
        m_activeTemplateId = id;
        m_currentTime = 0.0;
        emit activeTemplateIdChanged();
        emit durationChanged();
        emit currentTimeChanged();

        // Select first layer of this template
        QVariantMap tpl = getActiveTemplate();
        QVariantList layers = tpl.value("layers").toList();
        if (!layers.isEmpty()) {
            setSelectedLayerId(layers.first().toMap().value("id").toString());
        } else {
            setSelectedLayerId("");
        }
    }
}

void ProjectModel::setSelectedLayerId(const QString &id) {
    if (m_selectedLayerId != id) {
        m_selectedLayerId = id;
        emit selectedLayerIdChanged();
    }
}

void ProjectModel::setCurrentTime(double time) {
    double d = duration();
    double clamped = std::clamp(time, 0.0, d);
    if (std::abs(m_currentTime - clamped) > 0.001) {
        m_currentTime = clamped;
        emit currentTimeChanged();
    }
}

void ProjectModel::setIsPlaying(bool playing) {
    if (m_isPlaying != playing) {
        m_isPlaying = playing;
        emit isPlayingChanged();
    }
}

double ProjectModel::duration() const {
    QVariantMap tpl = getActiveTemplate();
    return tpl.value("duration", 5.0).toDouble();
}

QVariantMap ProjectModel::getActiveTemplate() const {
    for (const auto &item : m_templates) {
        QVariantMap map = item.toMap();
        if (map.value("id").toString() == m_activeTemplateId) {
            return map;
        }
    }
    if (!m_templates.isEmpty()) {
        return m_templates.first().toMap();
    }
    return QVariantMap();
}

QVariantMap ProjectModel::getSelectedLayer() const {
    QVariantMap tpl = getActiveTemplate();
    QVariantList layers = tpl.value("layers").toList();
    for (const auto &l : layers) {
        QVariantMap map = l.toMap();
        if (map.value("id").toString() == m_selectedLayerId) {
            return map;
        }
    }
    return QVariantMap();
}

void ProjectModel::moveLayer(const QString &templateId, const QString &layerId, double deltaX, double deltaY) {
    for (int tIdx = 0; tIdx < m_templates.size(); ++tIdx) {
        QVariantMap tpl = m_templates[tIdx].toMap();
        if (tpl.value("id").toString() == templateId) {
            QVariantList layers = tpl.value("layers").toList();
            for (int lIdx = 0; lIdx < layers.size(); ++lIdx) {
                QVariantMap layer = layers[lIdx].toMap();
                if (layer.value("id").toString() == layerId) {
                    QVariantMap transform = layer.value("transform").toMap();
                    double curX = transform.value("x").toDouble() + deltaX;
                    double curY = transform.value("y").toDouble() + deltaY;
                    transform["x"] = std::round(curX);
                    transform["y"] = std::round(curY);
                    layer["transform"] = transform;

                    // Synchronize motion path across all position keyframes
                    QVariantList keyframes = layer.value("keyframes").toList();
                    for (int kIdx = 0; kIdx < keyframes.size(); ++kIdx) {
                        QVariantMap kf = keyframes[kIdx].toMap();
                        QVariantMap props = kf.value("props").toMap();
                        if (props.contains("x")) {
                            props["x"] = std::round(props.value("x").toDouble() + deltaX);
                        }
                        if (props.contains("y")) {
                            props["y"] = std::round(props.value("y").toDouble() + deltaY);
                        }
                        kf["props"] = props;
                        keyframes[kIdx] = kf;
                    }
                    layer["keyframes"] = keyframes;

                    layers[lIdx] = layer;
                    tpl["layers"] = layers;
                    m_templates[tIdx] = tpl;

                    emit templatesChanged();
                    emit layerTransformed(layerId, std::round(curX), std::round(curY));
                    return;
                }
            }
        }
    }
}

void ProjectModel::updateLayerTransform(const QString &templateId, const QString &layerId, double x, double y, double width, double height) {
    for (int tIdx = 0; tIdx < m_templates.size(); ++tIdx) {
        QVariantMap tpl = m_templates[tIdx].toMap();
        if (tpl.value("id").toString() == templateId) {
            QVariantList layers = tpl.value("layers").toList();
            for (int lIdx = 0; lIdx < layers.size(); ++lIdx) {
                QVariantMap layer = layers[lIdx].toMap();
                if (layer.value("id").toString() == layerId) {
                    QVariantMap transform = layer.value("transform").toMap();
                    transform["x"] = std::round(x);
                    transform["y"] = std::round(y);
                    transform["width"] = std::round(width);
                    transform["height"] = std::round(height);
                    layer["transform"] = transform;
                    layers[lIdx] = layer;
                    tpl["layers"] = layers;
                    m_templates[tIdx] = tpl;
                    emit templatesChanged();
                    return;
                }
            }
        }
    }
}

void ProjectModel::updateLayerContent(const QString &templateId, const QString &layerId, const QString &text) {
    for (int tIdx = 0; tIdx < m_templates.size(); ++tIdx) {
        QVariantMap tpl = m_templates[tIdx].toMap();
        if (tpl.value("id").toString() == templateId) {
            QVariantList layers = tpl.value("layers").toList();
            for (int lIdx = 0; lIdx < layers.size(); ++lIdx) {
                QVariantMap layer = layers[lIdx].toMap();
                if (layer.value("id").toString() == layerId) {
                    QVariantMap content = layer.value("content").toMap();
                    content["text"] = text;
                    layer["content"] = content;
                    layers[lIdx] = layer;
                    tpl["layers"] = layers;
                    m_templates[tIdx] = tpl;
                    emit templatesChanged();
                    return;
                }
            }
        }
    }
}

void ProjectModel::updateLayerBinding(const QString &templateId, const QString &layerId, const QString &columnName) {
    for (int tIdx = 0; tIdx < m_templates.size(); ++tIdx) {
        QVariantMap tpl = m_templates[tIdx].toMap();
        if (tpl.value("id").toString() == templateId) {
            QVariantList layers = tpl.value("layers").toList();
            for (int lIdx = 0; lIdx < layers.size(); ++lIdx) {
                QVariantMap layer = layers[lIdx].toMap();
                if (layer.value("id").toString() == layerId) {
                    QVariantMap binding;
                    binding["column"] = columnName;
                    binding["fallback"] = layer.value("content").toMap().value("text");
                    layer["dataBinding"] = binding;
                    layers[lIdx] = layer;
                    tpl["layers"] = layers;
                    m_templates[tIdx] = tpl;
                    emit templatesChanged();
                    return;
                }
            }
        }
    }
}

void ProjectModel::updateKeyframeTime(const QString &templateId, const QString &layerId, const QString &kfId, double newTime) {
    for (int tIdx = 0; tIdx < m_templates.size(); ++tIdx) {
        QVariantMap tpl = m_templates[tIdx].toMap();
        if (tpl.value("id").toString() == templateId) {
            QVariantList layers = tpl.value("layers").toList();
            for (int lIdx = 0; lIdx < layers.size(); ++lIdx) {
                QVariantMap layer = layers[lIdx].toMap();
                if (layer.value("id").toString() == layerId) {
                    QVariantList keyframes = layer.value("keyframes").toList();
                    for (int kIdx = 0; kIdx < keyframes.size(); ++kIdx) {
                        QVariantMap kf = keyframes[kIdx].toMap();
                        if (kf.value("id").toString() == kfId) {
                            kf["time"] = std::clamp(newTime, 0.0, duration());
                            keyframes[kIdx] = kf;
                            layer["keyframes"] = keyframes;
                            layers[lIdx] = layer;
                            tpl["layers"] = layers;
                            m_templates[tIdx] = tpl;
                            emit templatesChanged();
                            return;
                        }
                    }
                }
            }
        }
    }
}

void ProjectModel::addKeyframeAtCurrentTime(const QString &templateId, const QString &layerId) {
    Q_UNUSED(templateId);
    Q_UNUSED(layerId);
    // Keyframe addition helper
}

void ProjectModel::importCsvData(const QString &datasetId, const QString &csvContent) {
    QStringList lines = csvContent.split('\n');
    if (lines.isEmpty()) return;

    QStringList headers = lines.first().trimmed().split(',');
    QVariantList rows;

    for (int i = 1; i < lines.size(); ++i) {
        QString line = lines[i].trimmed();
        if (line.isEmpty()) continue;
        QStringList cells = line.split(',');
        QVariantMap row;
        for (int h = 0; h < headers.size() && h < cells.size(); ++h) {
            row[headers[h].trimmed()] = cells[h].trimmed();
        }
        rows.append(row);
    }

    for (int i = 0; i < m_datasets.size(); ++i) {
        QVariantMap ds = m_datasets[i].toMap();
        if (ds.value("id").toString() == datasetId) {
            ds["headers"] = QVariant::fromValue(headers);
            ds["rows"] = rows;
            m_datasets[i] = ds;
            emit datasetsChanged();
            return;
        }
    }
}

QVariantList ProjectModel::getDatasetRows(const QString &datasetId) const {
    for (const auto &item : m_datasets) {
        QVariantMap ds = item.toMap();
        if (ds.value("id").toString() == datasetId) {
            return ds.value("rows").toList();
        }
    }
    return QVariantList();
}

void ProjectModel::initDefaultProject() {
    // 1. Datasets
    QVariantMap dsLeaderboard;
    dsLeaderboard["id"] = "ds_leaderboard";
    dsLeaderboard["name"] = "Klasemen Balap / Pertandingan";
    QStringList lbHeaders = {"Posisi", "Nama", "Klub/Tim", "Waktu/Poin"};
    dsLeaderboard["headers"] = QVariant::fromValue(lbHeaders);
    QVariantList lbRows = {
        QVariantMap{{"Posisi", "1"}, {"Nama", "Bambang Pamungkas"}, {"Klub/Tim", "Garuda FC"}, {"Waktu/Poin", "1:24.450"}},
        QVariantMap{{"Posisi", "2"}, {"Nama", "Reza Rahadian"}, {"Klub/Tim", "Speed Hunters"}, {"Waktu/Poin", "+0.124"}},
        QVariantMap{{"Posisi", "3"}, {"Nama", "Nicholas Saputra"}, {"Klub/Tim", "Apex Racing"}, {"Waktu/Poin", "+0.380"}},
        QVariantMap{{"Posisi", "4"}, {"Nama", "Iqbaal Ramadhan"}, {"Klub/Tim", "Redline Team"}, {"Waktu/Poin", "+0.720"}},
        QVariantMap{{"Posisi", "5"}, {"Nama", "Chicco Jerikho"}, {"Klub/Tim", "Turbo Velocity"}, {"Waktu/Poin", "+1.105"}}
    };
    dsLeaderboard["rows"] = lbRows;
    m_datasets.append(dsLeaderboard);

    // 2. Templates
    // Template 1: Lower Third News Modern
    QVariantMap tplLT;
    tplLT["id"] = "tpl_lower_third_news";
    tplLT["name"] = "Lower Third News Modern";
    tplLT["category"] = "lower-third";
    tplLT["defaultLayer"] = "L2";
    tplLT["duration"] = 5.0;

    QVariantMap markersLT;
    markersLT["introStart"] = 0.0;
    markersLT["introEnd"] = 0.8;
    markersLT["holdStart"] = 0.8;
    markersLT["holdEnd"] = 4.2;
    markersLT["outroStart"] = 4.2;
    markersLT["outroEnd"] = 5.0;
    tplLT["markers"] = markersLT;

    QVariantList layersLT;
    // Tag bar
    QVariantMap lTag;
    lTag["id"] = "layer_accent_bar";
    lTag["name"] = "Accent Pill / Category Tag";
    lTag["type"] = "shape";
    lTag["visible"] = true;
    lTag["locked"] = false;
    lTag["transform"] = QVariantMap{{"x", 120}, {"y", 830}, {"width", 180}, {"height", 32}, {"scaleX", 1}, {"scaleY", 1}, {"rotation", 0}};
    lTag["style"] = QVariantMap{{"fill", "#ff1744"}, {"borderRadius", 6}};
    layersLT.append(lTag);

    // Makasna Brand Icon
    QVariantMap lLogo;
    lLogo["id"] = "layer_accent_logo";
    lLogo["name"] = "Makasna Logo Mark";
    lLogo["type"] = "image";
    lLogo["visible"] = true;
    lLogo["locked"] = false;
    lLogo["transform"] = QVariantMap{{"x", 130}, {"y", 835}, {"width", 22}, {"height", 22}, {"scaleX", 1}, {"scaleY", 1}, {"rotation", 0}};
    lLogo["content"] = QVariantMap{{"src", "qrc:/makasna-logo.svg"}};
    layersLT.append(lLogo);

    // Headline Text
    QVariantMap lName;
    lName["id"] = "layer_headline_text";
    lName["name"] = "Headline / Nama Narasumber";
    lName["type"] = "text";
    lName["visible"] = true;
    lName["locked"] = false;
    lName["transform"] = QVariantMap{{"x", 140}, {"y", 874}, {"width", 720}, {"height", 44}, {"scaleX", 1}, {"scaleY", 1}, {"rotation", 0}};
    lName["style"] = QVariantMap{{"fill", "#ffffff"}, {"fontSize", 32}, {"fontWeight", 900}};
    lName["content"] = QVariantMap{{"text", "Dr. Ir. Rengga Pratama, M.Kom"}};
    layersLT.append(lName);

    // Subtitle Text
    QVariantMap lSub;
    lSub["id"] = "layer_subtitle_text";
    lSub["name"] = "Subtitle / Jabatan";
    lSub["type"] = "text";
    lSub["visible"] = true;
    lSub["locked"] = false;
    lSub["transform"] = QVariantMap{{"x", 140}, {"y", 926}, {"width", 720}, {"height", 28}, {"scaleX", 1}, {"scaleY", 1}, {"rotation", 0}};
    lSub["style"] = QVariantMap{{"fill", "#00e5ff"}, {"fontSize", 17}, {"fontWeight", 600}};
    lSub["content"] = QVariantMap{{"text", "Pakar Penyiaran Digital & AI Developer"}};
    layersLT.append(lSub);

    tplLT["layers"] = layersLT;
    m_templates.append(tplLT);

    // Template 2: Channel Bug Watermark (L4)
    QVariantMap tplBug;
    tplBug["id"] = "tpl_channel_bug";
    tplBug["name"] = "MAKASNA Permanent Channel Bug (L4)";
    tplBug["category"] = "channel-bug";
    tplBug["defaultLayer"] = "L4";
    tplBug["duration"] = 60.0;
    QVariantList layersBug;
    QVariantMap lBugLogo;
    lBugLogo["id"] = "layer_bug_logo";
    lBugLogo["name"] = "MAKASNA Logo";
    lBugLogo["type"] = "image";
    lBugLogo["visible"] = true;
    lBugLogo["locked"] = false;
    lBugLogo["transform"] = QVariantMap{{"x", 1680}, {"y", 65}, {"width", 52}, {"height", 52}, {"scaleX", 1}, {"scaleY", 1}, {"rotation", 0}};
    lBugLogo["content"] = QVariantMap{{"src", "qrc:/makasna-logo.svg"}};
    layersBug.append(lBugLogo);

    QVariantMap lBugText;
    lBugText["id"] = "layer_bug_text";
    lBugText["name"] = "MAKASNA HD LIVE";
    lBugText["type"] = "text";
    lBugText["visible"] = true;
    lBugText["locked"] = false;
    lBugText["transform"] = QVariantMap{{"x", 1740}, {"y", 80}, {"width", 140}, {"height", 24}, {"scaleX", 1}, {"scaleY", 1}, {"rotation", 0}};
    lBugText["style"] = QVariantMap{{"fill", "#ffffff"}, {"fontSize", 16}, {"fontWeight", 900}};
    lBugText["content"] = QVariantMap{{"text", "MAKASNA"}};
    layersBug.append(lBugText);
    tplBug["layers"] = layersBug;
    m_templates.append(tplBug);

    // 3. Rundown
    QVariantMap rItem1;
    rItem1["id"] = "item_1";
    rItem1["title"] = "Opening - Dr. Rengga Pratama";
    rItem1["templateId"] = "tpl_lower_third_news";
    rItem1["targetLayer"] = "L2";
    rItem1["status"] = "idle";
    m_rundown.append(rItem1);

    QVariantMap rItem2;
    rItem2["id"] = "item_bug";
    rItem2["title"] = "MAKASNA Channel Watermark (L4)";
    rItem2["templateId"] = "tpl_channel_bug";
    rItem2["targetLayer"] = "L4";
    rItem2["status"] = "idle";
    m_rundown.append(rItem2);

    m_activeTemplateId = "tpl_lower_third_news";
    m_selectedLayerId = "layer_headline_text";
}
