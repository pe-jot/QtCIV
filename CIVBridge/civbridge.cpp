#include "civbridge.h"
#include "ic705serial.h"
#include <QTcpSocket>
#include <QDebug>


const QString CIVBridge::websocketErrorCommand = QStringLiteral("error");
const QString CIVBridge::websocketRxFrequencyCommand = QStringLiteral("rxFreq");
const QString CIVBridge::websocketSetRxFrequencyCommand = QStringLiteral("setRxFreq");
const QString CIVBridge::websocketTxFrequencyCommand = QStringLiteral("txFreq");
const QString CIVBridge::websocketSquelchStatusCommand = QStringLiteral("squelchStatus");
const QString CIVBridge::websocketTxStatusCommand = QStringLiteral("txStatus");
const QString CIVBridge::websocketOvfStatusCommand = QStringLiteral("ovfStatus");
const QString CIVBridge::websocketSMeterCommand = QStringLiteral("sMeter");


CIVBridge::CIVBridge(const quint16& websocketPort, const quint16& talkkonnectPort, const quint8& voiceactivityPin, const quint8& heartbeatPin, const qint32& heartbeatTimeout, QObject *parent)
    : QObject(parent)
    , _raspiGpio(voiceactivityPin, heartbeatPin)
    , _pollTimer(new QTimer())
    , _watchdogTimer(new QTimer())
    , _websocketServer(new WebSocketServer(websocketPort))
    , _talkkonnect(new TalkkonnectClient(QHostAddress::LocalHost, talkkonnectPort))
    , _watchdogTimeoutMs(heartbeatTimeout)
{
    connect(_websocketServer, &WebSocketServer::commandReceived, this, &CIVBridge::onWebsocketCommandReceived);
    connect(_websocketServer, &WebSocketServer::clientConnected, this, &CIVBridge::onWebsocketClientConnected);

    _comm = new IC705Serial();
    connect(_comm, &IICOMcomm::initComplete, this, &CIVBridge::onCommInitDone);
    connect(_comm, &IICOMcomm::dataReceived, this, &CIVBridge::onCIVDataReceived);

    connect(_watchdogTimer, &QTimer::timeout, this, &CIVBridge::onWatchdogTimeout);
    _watchdogTimer->setSingleShot(true);
    _watchdogTimer->setInterval(_watchdogTimeoutMs);
}


CIVBridge::~CIVBridge()
{
    disconnect(_websocketServer, &WebSocketServer::commandReceived, this, &CIVBridge::onWebsocketCommandReceived);
    disconnect(_websocketServer, &WebSocketServer::clientConnected, this, &CIVBridge::onWebsocketClientConnected);
    _websocketServer->deleteLater();

    disconnect(_comm, &IICOMcomm::initComplete, this, &CIVBridge::onCommInitDone);
    disconnect(_comm, &IICOMcomm::dataReceived, this, &CIVBridge::onCIVDataReceived);
    _comm->deleteLater();

    disconnect(_watchdogTimer, &QTimer::timeout, this, &CIVBridge::onWatchdogTimeout);
    _watchdogTimer->deleteLater();

    _talkkonnect->deleteLater();
    _pollTimer->deleteLater();
}


void CIVBridge::onCommInitDone(const bool success)
{
    if (!success)
    {
        _websocketServer->broadcastUpdate(websocketErrorCommand, { "Connection failed! Refresh page to retry." });
        return;
    }

    if (!_pollTimer->isActive())
    {
        _pollTimer->setInterval(_pollIntervalMs);
        connect(_pollTimer, &QTimer::timeout, this, &CIVBridge::onPollInterval);
        _pollTimer->start();
    }
}


void CIVBridge::onCIVDataReceived(const QList<QByteArray> data)
{
    for (auto it = data.constBegin(); it != data.constEnd(); ++it)
    {
        auto rxFrequency = _civ.ParseRxFrequencyResponse(*it);
        if (rxFrequency != -1)
        {
            setRxFrequency(rxFrequency);
            continue;
        }

        auto txFrequency = _civ.ParseTxFrequencyResponse(*it);
        if (txFrequency != -1)
        {
            setTxFrequency(txFrequency);
            continue;
        }

        auto squelchStatus = _civ.ParseSquelchStatus(*it);
        if (squelchStatus != -1)
        {
            setSquelchStatus(squelchStatus);
            continue;
        }

        auto overflowStatus = _civ.ParseOvfStatus(*it);
        if (overflowStatus != -1)
        {
            setOverflowStatus(overflowStatus);
            continue;
        }

        auto sMeter = _civ.ParseSMeter(*it);
        if (sMeter != -1)
        {
            setSMeter(sMeter);
            continue;
        }

        if (*it == _civ.ResponseOK)
        {
            continue;
        }

        qDebug().nospace() << "Unknown data received (" << (*it).length() << " Bytes): " << *it;
    }
}


void CIVBridge::onPollInterval() const
{
    // Poll TRX status
    _comm->writeData(_civ.CmdReadRxFreq());
    _comm->writeData(_civ.CmdReadTxFreq());
    _comm->writeData(_civ.CmdReadSquelchStatus());
    _comm->writeData(_civ.CmdReadOvfStatus());
    _comm->writeData(_civ.CmdReadSMeter());

    // A voiceactivity pin toggle triggers the PTT on the TRX
    static bool oldVoiceActivityStatus = false;
    bool voiceActivityStatus = _raspiGpio.ReadVoiceactivityPin();
    if (voiceActivityStatus != oldVoiceActivityStatus && (_watchdogTimer->isActive() || _watchdogTimeoutMs <= 0))
    {
        _comm->writePTT(voiceActivityStatus);
        _websocketServer->broadcastUpdate(websocketTxStatusCommand, { voiceActivityStatus });
        oldVoiceActivityStatus = voiceActivityStatus;
    }

    if (_watchdogTimeoutMs > 0)
    {
        static bool oldHeartBeatStatus = false;
        // A heartbeat pin toggle restarts the watchdog timer
        bool heartBeatStatus = _raspiGpio.ReadHeartbeatPin();
        if (heartBeatStatus != oldHeartBeatStatus)
        {
            _watchdogTimer->start();
            oldHeartBeatStatus = heartBeatStatus;
        }
    }
}


void CIVBridge::onWatchdogTimeout() const
{
    qDebug() << "Heartbeat Watchdog timeout!";
}


void CIVBridge::setRxFrequency(int frequency)
{
    if (_rxFrequency != frequency)
    {
        _websocketServer->broadcastUpdate(websocketRxFrequencyCommand, { frequency });
        _rxFrequency = frequency;
    }
}


void CIVBridge::setTxFrequency(int frequency)
{
    if (_txFrequency != frequency)
    {
        _websocketServer->broadcastUpdate(websocketTxFrequencyCommand, { frequency });
        _txFrequency = frequency;
    }
}


void CIVBridge::setSquelchStatus(bool squelchStatus)
{
    if (_squelchStatus != squelchStatus)
    {
        _websocketServer->broadcastUpdate(websocketSquelchStatusCommand, { squelchStatus });
        _talkkonnect->sendTransmitCommand(squelchStatus);
        _squelchStatus = squelchStatus;
    }
}


void CIVBridge::setOverflowStatus(bool overflowStatus)
{
    if (_overflowStatus != overflowStatus)
    {
        _websocketServer->broadcastUpdate(websocketOvfStatusCommand, { overflowStatus });
        _overflowStatus = overflowStatus;
    }
}


void CIVBridge::setSMeter(int sMeter)
{
    if (_sMeter != sMeter)
    {
        _websocketServer->broadcastUpdate(websocketSMeterCommand, { sMeter, CIVProtocol::SMeterValueToText(sMeter) });
        _sMeter = sMeter;
    }
}


void CIVBridge::onWebsocketClientConnected()
{
    if (!_comm->isOpen())
    {
        _comm->init();
    }

    // Send initial values
    _websocketServer->broadcastUpdate(websocketRxFrequencyCommand, { _rxFrequency });
    _websocketServer->broadcastUpdate(websocketTxFrequencyCommand, { _txFrequency });
    _websocketServer->broadcastUpdate(websocketSquelchStatusCommand, { _squelchStatus });
    _websocketServer->broadcastUpdate(websocketOvfStatusCommand, { _overflowStatus });
    _websocketServer->broadcastUpdate(websocketSMeterCommand, { _sMeter, CIVProtocol::SMeterValueToText(_sMeter) });
}


void CIVBridge::onWebsocketCommandReceived(const QString name, const QVariant& arguments) const
{
    auto argumentMap = arguments.toMap();
    if (name == "setRxFreq" && argumentMap.contains("value"))
    {
        auto newRxFrequency = argumentMap.value("value").toInt();
        _comm->writeData(_civ.CmdWriteVfoFrequency(newRxFrequency, CIVProtocol::VFO::Selected));
    }
}
