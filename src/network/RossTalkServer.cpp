#include "RossTalkServer.h"
#include "../core/PlayoutController.h"
#include <QDebug>

RossTalkServer::RossTalkServer(PlayoutController *controller, QObject *parent)
    : QObject(parent), m_controller(controller) {
    m_server = new QTcpServer(this);
    connect(m_server, &QTcpServer::newConnection, this, &RossTalkServer::onNewConnection);
}

bool RossTalkServer::start(quint16 port) {
    if (!m_server->listen(QHostAddress::Any, port)) {
        qWarning() << "[RossTalk] Failed to bind to port" << port << ":" << m_server->errorString();
        return false;
    }
    qDebug() << "[RossTalk] Automation TCP Server listening on port" << port;
    return true;
}

void RossTalkServer::stop() {
    for (auto *client : m_clients) {
        client->disconnectFromHost();
    }
    m_clients.clear();
    m_server->close();
}

void RossTalkServer::onNewConnection() {
    QTcpSocket *client = m_server->nextPendingConnection();
    m_clients.append(client);
    connect(client, &QTcpSocket::readyRead, this, &RossTalkServer::onReadyRead);
    connect(client, &QTcpSocket::disconnected, this, &RossTalkServer::onClientDisconnected);
    qDebug() << "[RossTalk] Client connected from" << client->peerAddress().toString();
}

void RossTalkServer::onReadyRead() {
    auto *client = qobject_cast<QTcpSocket*>(sender());
    if (!client) return;

    while (client->canReadLine()) {
        QString line = QString::fromUtf8(client->readLine()).trimmed();
        if (!line.isEmpty()) {
            processCommand(line);
        }
    }
}

void RossTalkServer::onClientDisconnected() {
    auto *client = qobject_cast<QTcpSocket*>(sender());
    if (client) {
        m_clients.removeAll(client);
        client->deleteLater();
        qDebug() << "[RossTalk] Client disconnected";
    }
}

void RossTalkServer::processCommand(const QString &command) {
    qDebug() << "[RossTalk] Received command:" << command;
    QStringList parts = command.split(' ', Qt::SkipEmptyParts);
    if (parts.isEmpty()) return;

    QString verb = parts[0].toUpper();

    if (verb == "TAKE") {
        if (m_controller) m_controller->take();
    } else if (verb == "CLEAR" || verb == "CLR") {
        if (parts.size() > 1) {
            QString layer = parts[1].toUpper();
            if (m_controller) m_controller->clearLayer(layer);
        } else {
            if (m_controller) m_controller->clearAll();
        }
    } else if (verb == "CUE") {
        if (parts.size() > 1 && m_controller) {
            m_controller->cueItem(parts[1]);
        }
    } else if (verb == "CLOCK") {
        if (parts.size() > 1 && m_controller) {
            QString action = parts[1].toUpper();
            if (action == "START") m_controller->startClock();
            else if (action == "PAUSE" || action == "STOP") m_controller->pauseClock();
            else if (action == "RESET") m_controller->resetClock();
        }
    } else if (verb == "GOAL") {
        if (parts.size() > 1 && m_controller) {
            QString team = parts[1].toUpper();
            if (team == "HOME") m_controller->adjustScore(true, 1);
            else if (team == "AWAY") m_controller->adjustScore(false, 1);
        }
    }
}
