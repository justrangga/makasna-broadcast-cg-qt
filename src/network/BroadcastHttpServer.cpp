#include "BroadcastHttpServer.h"
#include <QDebug>

BroadcastHttpServer::BroadcastHttpServer(QObject *parent) : QObject(parent) {
    m_server = new QTcpServer(this);
    connect(m_server, &QTcpServer::newConnection, this, &BroadcastHttpServer::onNewConnection);
}

bool BroadcastHttpServer::start(quint16 port) {
    if (!m_server->listen(QHostAddress::Any, port)) {
        qWarning() << "[HttpServer] Failed to bind to port" << port << ":" << m_server->errorString();
        return false;
    }
    qDebug() << "[HttpServer] Broadcast Playout Server listening on port" << port;
    return true;
}

void BroadcastHttpServer::stop() {
    m_server->close();
}

void BroadcastHttpServer::onNewConnection() {
    QTcpSocket *socket = m_server->nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead, this, &BroadcastHttpServer::onReadyRead);
}

void BroadcastHttpServer::onReadyRead() {
    auto *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    QByteArray request = socket->readAll();
    // Return transparent 1080p broadcast canvas HTML
    QString html = R"HTML(<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<title>MAKASNA Broadcast CG Output</title>
<style>
  html, body {
    margin: 0; padding: 0;
    width: 1920px; height: 1080px;
    background: transparent !important;
    overflow: hidden;
    font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif;
  }
</style>
</head>
<body>
  <div id="output-root" style="position:relative; width:1920px; height:1080px; background:transparent;"></div>
</body>
</html>)HTML";

    QByteArray response = "HTTP/1.1 200 OK\r\n"
                          "Content-Type: text/html; charset=utf-8\r\n"
                          "Access-Control-Allow-Origin: *\r\n"
                          "Connection: close\r\n"
                          "Content-Length: " + QByteArray::number(html.toUtf8().size()) + "\r\n\r\n"
                          + html.toUtf8();

    socket->write(response);
    socket->flush();
    socket->disconnectFromHost();
}
