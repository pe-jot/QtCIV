#include "talkkonnectclient.h"


const int HttpClientWorker::socketTimeout = 3000;


HttpClientWorker::HttpClientWorker(const QHostAddress& host, const quint16& port, const QString& httpCommand)
    : _httpHost(host)
    , _httpPort(port)
    , _httpCommand(httpCommand)
{
}


HttpClientWorker::~HttpClientWorker()
{
}


void HttpClientWorker::run()
{
    _socket = new QTcpSocket();
    _socket->connectToHost(_httpHost, _httpPort);
    if (!_socket->waitForConnected(socketTimeout))
    {
        qDebug() << "Failed to connect to Talkkonnect HTTP service - error" << _socket->error() << _socket->errorString();
        emit workerComplete(false);
        return;
    }

    _socket->write(_httpCommand.toLatin1());
    if (!_socket->waitForBytesWritten(socketTimeout))
    {
        qDebug() << "Failed to write to Talkkonnect HTTP service - error" << _socket->error() << _socket->errorString();
        emit workerComplete(false);
        return;
    }

    _socket->close();
    emit workerComplete(true);
    _socket->deleteLater();
}


TalkkonnectClient::TalkkonnectClient(const QHostAddress& host, const quint16& port, QObject *parent)
    : QObject(parent)
    , _httpHost(host)
    , _httpPort(port)
    , _transmitDropoutDelay(new QTimer())
{
	connect(_transmitDropoutDelay, &QTimer::timeout, this, &TalkkonnectClient::onDropoutDelayElapsed);
    _transmitDropoutDelay->setSingleShot(true);
}


TalkkonnectClient::~TalkkonnectClient()
{
	disconnect(_transmitDropoutDelay, &QTimer::timeout, this, &TalkkonnectClient::onDropoutDelayElapsed);
    _transmitDropoutDelay->deleteLater();
    _worker->deleteLater();
}


void TalkkonnectClient::onDropoutDelayElapsed()
{
    if (_stopCommandBlocked)
    {
        _stopCommandBlocked = false;
        // Actually issue HTTP command
        sendTransmitCommand(false, 0);
    }
}


void TalkkonnectClient::sendTransmitCommand(const bool &start, const int& dropOutDelayMsec)
{
    static bool oldStart = false;

    if (start)
    {
        _stopCommandBlocked = false;
        _transmitDropoutDelay->start(dropOutDelayMsec);
    }
    else if (_transmitDropoutDelay->isActive())
    {
        // Filter too fast stop command after start command
        _stopCommandBlocked = true;
        return;
    }

    // Filter subsequent start commands
    if (start && start == oldStart)
    {
        return;
    }
    oldStart = start;

    auto httpCommand = QString("GET /?command=%1Transmitting HTTP/1.1\r\nHost: %2:%3\r\nConnection: close\r\n\r\n")
            .arg(start ? "Start" : "Stop")
            .arg(_httpHost.toIPv4Address())
            .arg(_httpPort);

    HttpClientWorker* worker = new HttpClientWorker(_httpHost, _httpPort, httpCommand);
    connect(worker, &HttpClientWorker::finished, worker, &QObject::deleteLater);
    worker->start();
}
