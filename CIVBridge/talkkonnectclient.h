#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QThread>
#include <QTimer>

class HttpClientWorker : public QThread
{
    Q_OBJECT

public:
    HttpClientWorker(const QHostAddress& host, const quint16& port, const QString& command);
    HttpClientWorker(const HttpClientWorker&) = delete;
    HttpClientWorker& operator=(const HttpClientWorker&) = delete;
    ~HttpClientWorker();

    void run() override;

signals:
    void workerComplete(const bool success);

private:
    static const int socketTimeout;

    const QHostAddress& _httpHost;
    const quint16& _httpPort;
    // Must not a be reference because we get it from the parent thread!
    const QString _httpCommand;

    QTcpSocket* _socket = nullptr;
};



class TalkkonnectClient : public QObject
{
    Q_OBJECT

public:
    TalkkonnectClient(const QHostAddress& host, const quint16& port, QObject *parent = nullptr);
    TalkkonnectClient(const TalkkonnectClient&) = delete;
    TalkkonnectClient& operator=(const TalkkonnectClient&) = delete;
    ~TalkkonnectClient();

    void sendTransmitCommand(const bool& start, const int& dropOutDelayMsec = 3000);

private slots:
    void onDropoutDelayElapsed();

private:
    // Must not be references because we pass them to the child thread
    // and we get them already as reference from the main.cpp!
    const QHostAddress _httpHost;
    const quint16 _httpPort;

    bool _stopCommandBlocked = false;

    HttpClientWorker* _worker = nullptr;
    QTimer* _transmitDropoutDelay = nullptr;
};

