#pragma once

#include <QObject>
#include <QString>
#include <QVariantList>
#include <QVariantMap>
#include <QTimer>

class DeckLinkController : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isStreaming READ isStreaming NOTIFY isStreamingChanged)
    Q_PROPERTY(QString selectedDevice READ selectedDevice NOTIFY selectedDeviceChanged)
    Q_PROPERTY(QString videoStandard READ videoStandard NOTIFY videoStandardChanged)
    Q_PROPERTY(QString keyerMode READ keyerMode NOTIFY keyerModeChanged)
    Q_PROPERTY(int droppedFrames READ droppedFrames NOTIFY droppedFramesChanged)
    Q_PROPERTY(quint64 outputFrames READ outputFrames NOTIFY outputFramesChanged)
    Q_PROPERTY(bool genlockLocked READ genlockLocked NOTIFY genlockLockedChanged)

public:
    explicit DeckLinkController(QObject *parent = nullptr);

    bool isStreaming() const { return m_isStreaming; }
    QString selectedDevice() const { return m_selectedDevice; }
    QString videoStandard() const { return m_videoStandard; }
    QString keyerMode() const { return m_keyerMode; }
    int droppedFrames() const { return m_droppedFrames; }
    quint64 outputFrames() const { return m_outputFrames; }
    bool genlockLocked() const { return m_genlockLocked; }

    Q_INVOKABLE QVariantList getDevices() const;
    Q_INVOKABLE bool startPlayout(int deviceIndex, const QString &standard, const QString &keyer);
    Q_INVOKABLE void stopPlayout();

signals:
    void isStreamingChanged();
    void selectedDeviceChanged();
    void videoStandardChanged();
    void keyerModeChanged();
    void droppedFramesChanged();
    void outputFramesChanged();
    void genlockLockedChanged();

private slots:
    void onFrameTick();

private:
    bool m_isStreaming = false;
    QString m_selectedDevice;
    QString m_videoStandard = "1080p60";
    QString m_keyerMode = "external";
    int m_droppedFrames = 0;
    quint64 m_outputFrames = 0;
    bool m_genlockLocked = true;
    QTimer *m_frameTimer = nullptr;
};
