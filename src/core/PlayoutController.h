#pragma once

#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QTimer>

class PlayoutController : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString cuedItemId READ cuedItemId NOTIFY cuedItemIdChanged)
    Q_PROPERTY(QVariantMap activeLayers READ activeLayers NOTIFY activeLayersChanged)
    Q_PROPERTY(int homeScore READ homeScore NOTIFY homeScoreChanged)
    Q_PROPERTY(int awayScore READ awayScore NOTIFY awayScoreChanged)
    Q_PROPERTY(int matchClockSeconds READ matchClockSeconds NOTIFY matchClockSecondsChanged)
    Q_PROPERTY(bool isClockRunning READ isClockRunning NOTIFY isClockRunningChanged)
    Q_PROPERTY(QString clockString READ clockString NOTIFY clockStringChanged)

public:
    explicit PlayoutController(QObject *parent = nullptr);

    QString cuedItemId() const { return m_cuedItemId; }
    QVariantMap activeLayers() const { return m_activeLayers; }
    int homeScore() const { return m_homeScore; }
    int awayScore() const { return m_awayScore; }
    int matchClockSeconds() const { return m_matchClockSeconds; }
    bool isClockRunning() const { return m_isClockRunning; }
    QString clockString() const;

    Q_INVOKABLE void cueItem(const QString &itemId);
    Q_INVOKABLE void take();
    Q_INVOKABLE void takeItem(const QString &itemId, const QString &templateId, const QString &targetLayer);
    Q_INVOKABLE void clearLayer(const QString &layer);
    Q_INVOKABLE void clearAll();

    // Rapid Sports Control
    Q_INVOKABLE void setScores(int home, int away);
    Q_INVOKABLE void adjustScore(bool isHome, int delta);
    Q_INVOKABLE void startClock();
    Q_INVOKABLE void pauseClock();
    Q_INVOKABLE void resetClock();
    Q_INVOKABLE void addClockSeconds(int seconds);

signals:
    void cuedItemIdChanged();
    void activeLayersChanged();
    void homeScoreChanged();
    void awayScoreChanged();
    void matchClockSecondsChanged();
    void isClockRunningChanged();
    void clockStringChanged();
    void playoutEventFired(const QString &eventType, const QVariantMap &payload);

private slots:
    void onClockTick();

private:
    QString m_cuedItemId = "item_1";
    QVariantMap m_activeLayers; // L1, L2, L3, L4
    int m_homeScore = 0;
    int m_awayScore = 0;
    int m_matchClockSeconds = 0;
    bool m_isClockRunning = false;
    QTimer *m_clockTimer = nullptr;
};
