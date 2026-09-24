#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>

class PlayoutController;

class RossTalkServer : public QObject {
    Q_OBJECT
public:
    explicit RossTalkServer(PlayoutController *controller, QObject *parent = nullptr);
    bool start(quint16 port = 7788);
    void stop();

private slots:
    void onNewConnection();
    void onReadyRead();
    void onClientDisconnected();

private:
    void processCommand(const QString &command);

    QTcpServer *m_server = nullptr;
    QList<QTcpSocket*> m_clients;
    PlayoutController *m_controller = nullptr;
};
