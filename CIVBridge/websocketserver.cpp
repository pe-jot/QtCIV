#include "websocketserver.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QDebug>


static QString getClientIdentifier(const QWebSocket *peer)
{
    return QStringLiteral("%1:%2").arg(peer->peerAddress().toString(), QString::number(peer->peerPort()));
}


WebSocketServer::WebSocketServer(const quint16 port, const quint16 maxClients, QObject *parent)
    : QObject(parent)
    , _webSocketServer(new QWebSocketServer(QStringLiteral("WebsocketServer"), QWebSocketServer::NonSecureMode, this))
    , _maxClients(maxClients)
{
    if (_webSocketServer->listen(QHostAddress::Any, port))
    {
        qDebug() << "Listening on port" << port;
        connect(_webSocketServer, &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnection);
    }
    else
    {
        qDebug() << "Error: " << _webSocketServer->errorString();
    }
}


WebSocketServer::~WebSocketServer()
{
    _webSocketServer->close();
}


void WebSocketServer::onNewConnection()
{
    auto socket = _webSocketServer->nextPendingConnection();
    socket->setParent(this);

    if (_connectedClients.count() >= _maxClients)
    {
        qDebug() << "Denied connection from" << getClientIdentifier(socket);
        socket->close(QWebSocketProtocol::CloseCode::CloseCodeNormal, QStringLiteral("Too many clients connected"));
        socket->deleteLater();
        return;
    }

    qDebug() << "Accept connection from" << getClientIdentifier(socket);

    connect(socket, &QWebSocket::textMessageReceived, this, &WebSocketServer::onTextMessageReceived);
    connect(socket, &QWebSocket::disconnected, this, &WebSocketServer::onDisconnected);

    _connectedClients << socket;

    emit clientConnected();
}


void WebSocketServer::onTextMessageReceived(const QString &message)
{
    auto jsonObject = QJsonDocument::fromJson(message.toLatin1()).object();
    if (jsonObject.isEmpty())
    {
        return;
    }

    auto commandName = jsonObject.value("command");
    if (!commandName.isUndefined())
    {
        auto arguments = jsonObject.value("arguments").toVariant();
        emit commandReceived(commandName.toString(), arguments);
        return;
    }

    qDebug() << "Received unhandled websocket message:" << message;
}


void WebSocketServer::broadcastUpdate(const QString name, const QJsonArray arguments) const
{
    QJsonObject jsonObject;
    jsonObject.insert("name", name);
    jsonObject.insert("arguments", arguments);

    QJsonDocument jsonDocument(jsonObject);

    auto json = jsonDocument.toJson(QJsonDocument::JsonFormat::Compact);

    for (auto it = _connectedClients.constBegin(); it != _connectedClients.constEnd(); ++it)
    {
        (*it)->sendTextMessage(json);
    }
}


void WebSocketServer::disconnectAll()
{
    qDebug() << "Disconnect all" << _connectedClients.length() << "clients";

    for (auto it = _connectedClients.constBegin(); it != _connectedClients.constEnd(); ++it)
    {
        disconnect(*it, &QWebSocket::textMessageReceived, this, &WebSocketServer::onTextMessageReceived);
        disconnect(*it, &QWebSocket::disconnected, this, &WebSocketServer::onDisconnected);
        (*it)->close();
        (*it)->deleteLater();
    }

    _connectedClients.clear();
}


void WebSocketServer::onDisconnected()
{
    auto client = static_cast<QWebSocket*>(sender());
    if (!client)
    {
        return;
    }

    qDebug() << getClientIdentifier(client) << "disconnecting...";
    _connectedClients.removeAll(client);
    client->deleteLater();
}
