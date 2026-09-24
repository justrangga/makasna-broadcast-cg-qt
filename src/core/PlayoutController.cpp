#include "PlayoutController.h"
#include <QDateTime>
#include <QDebug>

PlayoutController::PlayoutController(QObject *parent) : QObject(parent) {
    m_clockTimer = new QTimer(this);
    m_clockTimer->setInterval(1000);
    connect(m_clockTimer, &QTimer::timeout, this, &PlayoutController::onClockTick);

    m_activeLayers["L1"] = QVariant();
    m_activeLayers["L2"] = QVariant();
    m_activeLayers["L3"] = QVariant();
    m_activeLayers["L4"] = QVariant();
}

QString PlayoutController::clockString() const {
    int m = m_matchClockSeconds / 60;
    int s = m_matchClockSeconds % 60;
    return QString("%1:%2")
        .arg(m, 2, 10, QChar('0'))
        .arg(s, 2, 10, QChar('0'));
}

void PlayoutController::cueItem(const QString &itemId) {
    if (m_cuedItemId != itemId) {
        m_cuedItemId = itemId;
        emit cuedItemIdChanged();
        QVariantMap payload;
        payload["cuedItemId"] = itemId;
        emit playoutEventFired("CUE", payload);
    }
}

void PlayoutController::take() {
    if (m_cuedItemId.isEmpty()) return;
    // Broadcast take event
    QVariantMap payload;
    payload["cuedItemId"] = m_cuedItemId;
    emit playoutEventFired("TAKE", payload);
}

void PlayoutController::takeItem(const QString &itemId, const QString &templateId, const QString &targetLayer) {
    QVariantMap itemData;
    itemData["id"] = itemId;
    itemData["templateId"] = templateId;
    itemData["targetLayer"] = targetLayer;
    itemData["onAirTime"] = QDateTime::currentMSecsSinceEpoch();

    m_activeLayers[targetLayer] = itemData;
    emit activeLayersChanged();

    QVariantMap payload;
    payload["targetLayer"] = targetLayer;
    payload["item"] = itemData;
    emit playoutEventFired("TAKE_LAYER", payload);
}

void PlayoutController::clearLayer(const QString &layer) {
    if (m_activeLayers.contains(layer) && !m_activeLayers.value(layer).isNull()) {
        m_activeLayers[layer] = QVariant();
        emit activeLayersChanged();

        QVariantMap payload;
        payload["layer"] = layer;
        emit playoutEventFired("CLEAR_LAYER", payload);
    }
}

void PlayoutController::clearAll() {
    m_activeLayers["L1"] = QVariant();
    m_activeLayers["L2"] = QVariant();
    m_activeLayers["L3"] = QVariant();
    m_activeLayers["L4"] = QVariant();
    emit activeLayersChanged();

    QVariantMap payload;
    payload["layer"] = "ALL";
    emit playoutEventFired("CLEAR_ALL", payload);
}

void PlayoutController::setScores(int home, int away) {
    m_homeScore = std::max(0, home);
    m_awayScore = std::max(0, away);
    emit homeScoreChanged();
    emit awayScoreChanged();
}

void PlayoutController::adjustScore(bool isHome, int delta) {
    if (isHome) {
        m_homeScore = std::max(0, m_homeScore + delta);
        emit homeScoreChanged();
    } else {
        m_awayScore = std::max(0, m_awayScore + delta);
        emit awayScoreChanged();
    }
}

void PlayoutController::startClock() {
    m_isClockRunning = true;
    m_clockTimer->start();
    emit isClockRunningChanged();
}

void PlayoutController::pauseClock() {
    m_isClockRunning = false;
    m_clockTimer->stop();
    emit isClockRunningChanged();
}

void PlayoutController::resetClock() {
    m_isClockRunning = false;
    m_clockTimer->stop();
    m_matchClockSeconds = 0;
    emit isClockRunningChanged();
    emit matchClockSecondsChanged();
    emit clockStringChanged();
}

void PlayoutController::addClockSeconds(int seconds) {
    m_matchClockSeconds = std::max(0, m_matchClockSeconds + seconds);
    emit matchClockSecondsChanged();
    emit clockStringChanged();
}

void PlayoutController::onClockTick() {
    if (m_isClockRunning) {
        m_matchClockSeconds++;
        emit matchClockSecondsChanged();
        emit clockStringChanged();
    }
}
