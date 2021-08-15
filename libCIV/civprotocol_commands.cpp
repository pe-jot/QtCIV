#include "civprotocol.h"


QByteArray CIVProtocol::CmdReadTransceiverId() const
{
    return CreateCommand(_cmdReadTransceiverId);
}


QByteArray CIVProtocol::CmdReadSquelchStatus() const
{
    return CreateCommand(_cmdReadSquelchStatus);
}


QByteArray CIVProtocol::CmdReadOvfStatus() const
{
    return CreateCommand(_cmdReadOvfStatus);
}


QByteArray CIVProtocol::CmdReadSMeter() const
{
    return CreateCommand(_cmdReadSMeter);
}


QByteArray CIVProtocol::CmdReadRxFreq() const
{
    return CreateCommand(_cmdReadRxFrequency);
}


QByteArray CIVProtocol::CmdReadTxFreq() const
{
    return CreateCommand(_cmdReadTxFrequency);
}


QByteArray CIVProtocol::CmdWriteVfoFrequency(const qint32 frequency, const VFO vfo) const
{
    auto command = QByteArray(_cmdWriteVfoFrequency).append(static_cast<char>(vfo));
    auto frequencyData = CreateFrequencyData(frequency);
    return CreateCommand(command, frequencyData);
}


QByteArray CIVProtocol::CmdSetGpsPower(const bool on) const
{
    auto data = on ? QByteArrayLiteral("\x01") : QByteArrayLiteral("\x00");
    return CreateCommand(_cmdSetGpsPower, data);
}


QByteArray CIVProtocol::CmdGetGpsStatus() const
{
    return CreateCommand(_cmdGetGpsStatus);
}


QByteArray CIVProtocol::ResponseReadTransceiverId() const
{
    return CreateResponse(_cmdReadTransceiverId, _transceiverAddress);
}
