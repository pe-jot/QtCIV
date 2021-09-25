#pragma once
#include <QObject>
#include <QTimer>
#include <QHostAddress>

#include "civprotocol.h"
#include "IICOMcomm.h"
#include "websocketserver.h"
#include "raspigpio.h"
#include "talkkonnectclient.h"

class CIVBridge : public QObject
{
    Q_OBJECT

public:
    explicit CIVBridge(const quint16& websocketPort = 88, const quint16& talkkonnectPort = 81, const quint8& voiceactivityPin = 5, const quint8& heartbeatPin = 25, QObject *parent = nullptr);
    CIVBridge(const CIVBridge&) = delete;
    CIVBridge& operator=(const CIVBridge&) = delete;
    ~CIVBridge();

private slots:
    void onWebsocketCommandReceived(const QString name, const QVariant& arguments) const;
    void onWebsocketClientConnected();
    void onCommInitDone(const bool success);
    void onCIVDataReceived(const QList<QByteArray> data);
    void onPollInterval() const;
    void onWatchdogTimeout() const;

private:
    void sendTalkkonnectTransmitCommand(const bool& start) const;
    void setRxFrequency(int frequency);
    void setTxFrequency(int frequency);
    void setSquelchStatus(bool squelchStatus);
    void setOverflowStatus(bool overflowStatus);
    void setSMeter(int sMeter);

    CIVProtocol _civ;
    RaspiGpio _raspiGpio;
    QTimer* _pollTimer;
    QTimer* _watchdogTimer;
    WebSocketServer* _websocketServer = nullptr;
    IICOMcomm* _comm = nullptr;
    TalkkonnectClient* _talkkonnect = nullptr;

    int _rxFrequency = 0;
    int _txFrequency = 0;
    bool _squelchStatus = false;
    bool _overflowStatus = false;
    int _sMeter = 0;

    const int _pollIntervalMs = 100;
    const int _watchdogTimeoutMs = 3000;

    static const QString websocketErrorCommand;
    static const QString websocketRxFrequencyCommand;
    static const QString websocketSetRxFrequencyCommand;
    static const QString websocketTxFrequencyCommand;
    static const QString websocketSquelchStatusCommand;
    static const QString websocketTxStatusCommand;
    static const QString websocketOvfStatusCommand;
    static const QString websocketSMeterCommand;
};

