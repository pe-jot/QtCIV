#pragma once

#include <QWebSocketServer>
#include <QWebSocket>
#include <QJsonArray>

class WebSocketServer : public QObject
{
    Q_OBJECT

public:
    WebSocketServer(const quint16 port, const quint16 maxClients = 1, QObject *parent = nullptr);
    WebSocketServer(const WebSocketServer&) = delete;
    WebSocketServer& operator=(const WebSocketServer&) = delete;
    ~WebSocketServer();

    void disconnectAll();
    void broadcastUpdate(const QString name, const QJsonArray arguments = {}) const;

signals:
    void clientConnected();
    void commandReceived(const QString name, const QVariant& arguments);

private slots:
    void onNewConnection();
    void onTextMessageReceived(const QString &message);
    void onDisconnected();

private:
    QWebSocketServer* _webSocketServer;
    QList<QWebSocket*> _connectedClients;
    const quint16 _maxClients;
};
