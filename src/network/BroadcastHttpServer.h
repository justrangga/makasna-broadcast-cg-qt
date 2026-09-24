#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class BroadcastHttpServer : public QObject {
    Q_OBJECT
public:
    explicit BroadcastHttpServer(QObject *parent = nullptr);
    bool start(quint16 port = 4989);
    void stop();

private slots:
    void onNewConnection();
    void onReadyRead();

private:
    QTcpServer *m_server = nullptr;
};
