#pragma once
#include <QByteArray>
#include "gpsinfo.h"

class CIVProtocol
{
    friend class CIVProtocol_Private_Test;

private:
    static const QByteArray _preamble;
    static const QByteArray _endOfMessage;
    static const QByteArray _broadcastAddress;

    static const QByteArray _cmdReadSquelchStatus;
    static const QByteArray _cmdReadSMeter;
    static const QByteArray _cmdReadOvfStatus;
    static const QByteArray _cmdReadTransceiverId;
    static const QByteArray _cmdReadTxFrequency;
    static const QByteArray _cmdReadRxFrequency;
    static const QByteArray _cmdRxFrequencyChange;
    static const QByteArray _cmdWriteVfoFrequency;
    static const QByteArray _cmdGetGpsStatus;
    static const QByteArray _cmdSetGpsPower;

    const QByteArray _transceiverAddress;
    const QByteArray _controllerAddress;
    const int _overheadLength;

    bool CheckResponse(const QByteArray& data, const QByteArray& expectedCommand, const quint8 expectedPayloadLength) const;
    QByteArray CreateFrequencyData(const qint32 frequency) const;
    qint32 ParseFrequencyData(const QByteArray& data) const;

public:
    enum class VFO : char { Selected = 0, Unselected = 1 };

    CIVProtocol(uchar transceiverAddress = 0xA4, uchar controllerAddress = 0xE0);
    ~CIVProtocol();

    QByteArray ResponseOK;
    QByteArray ResponseNG;

    QByteArray CreateCommand(const QByteArray& cmdData, const QByteArray& data = QByteArray()) const;
    QByteArray CreateResponse(const QByteArray& cmdData, const QByteArray& data = QByteArray()) const;

    QByteArray CmdReadTransceiverId() const;
    QByteArray CmdReadSquelchStatus() const;
    QByteArray CmdReadOvfStatus() const;
    QByteArray CmdReadSMeter() const;
    QByteArray CmdReadRxFreq() const;
    QByteArray CmdReadTxFreq() const;
    QByteArray CmdWriteVfoFrequency(const qint32 frequency, const VFO vfo) const;
    QByteArray CmdSetGpsPower(const bool on) const;
    QByteArray CmdGetGpsStatus() const;
    QByteArray ResponseReadTransceiverId() const;

    qint32 ParseRxFrequencyResponse(const QByteArray& data) const;
    qint32 ParseTxFrequencyResponse(const QByteArray& data) const;
    qint32 ParseRxFrequencyChange(const QByteArray& data) const;
    qint8 ParseSquelchStatus(const QByteArray& data) const;
    qint8 ParseOvfStatus(const QByteArray& data) const;
    qint32 ParseSMeter(const QByteArray& data) const;
    GPSInfo* ParseGpsStatusResponseData(const QByteArray& data) const;

    static QString SMeterValueToText(const quint8);
};
