#include "civprotocol.h"
#include <QString>

const QByteArray CIVProtocol::_preamble =                   QByteArrayLiteral("\xFE\xFE");
const QByteArray CIVProtocol::_endOfMessage =               QByteArrayLiteral("\xFD");
const QByteArray CIVProtocol::_broadcastAddress =           QByteArrayLiteral("\x00");

const QByteArray CIVProtocol::_cmdReadSquelchStatus =       QByteArrayLiteral("\x15\x01");
const QByteArray CIVProtocol::_cmdReadSMeter =              QByteArrayLiteral("\x15\x02");
const QByteArray CIVProtocol::_cmdReadOvfStatus =           QByteArrayLiteral("\x15\x07");
const QByteArray CIVProtocol::_cmdReadTransceiverId =       QByteArrayLiteral("\x19\x00");
const QByteArray CIVProtocol::_cmdReadTxFrequency =         QByteArrayLiteral("\x1C\x03");
const QByteArray CIVProtocol::_cmdReadRxFrequency =         QByteArrayLiteral("\x03");
const QByteArray CIVProtocol::_cmdRxFrequencyChange =       QByteArrayLiteral("\x00");
const QByteArray CIVProtocol::_cmdWriteVfoFrequency =       QByteArrayLiteral("\x25");
const QByteArray CIVProtocol::_cmdGetGpsStatus =            QByteArrayLiteral("\x23\x00");
const QByteArray CIVProtocol::_cmdSetGpsPower =             QByteArrayLiteral("\x23\x01");


CIVProtocol::CIVProtocol(uchar transceiverAddress, uchar controllerAddress)
    : _transceiverAddress(1, transceiverAddress)
    , _controllerAddress(1, controllerAddress)
    , _overheadLength(_preamble.length() + _transceiverAddress.length() + _controllerAddress.length() + _endOfMessage.length())
{
    ResponseOK = CreateResponse(QByteArrayLiteral("\xFB"));
    ResponseNG = CreateResponse(QByteArrayLiteral("\xFA"));
}


CIVProtocol::~CIVProtocol()
{
}


QByteArray CIVProtocol::CreateCommand(const QByteArray& cmdData, const QByteArray& data) const
{
    return QByteArray(_preamble)
            .append(_transceiverAddress)
            .append(_controllerAddress)
            .append(cmdData)
            .append(data)
            .append(_endOfMessage);
}


QByteArray CIVProtocol::CreateResponse(const QByteArray& cmdData, const QByteArray& data) const
{
    return QByteArray(_preamble)
            .append(_controllerAddress)
            .append(_transceiverAddress)
            .append(cmdData)
            .append(data)
            .append(_endOfMessage);
}


bool CIVProtocol::CheckResponse(const QByteArray& data, const QByteArray& expectedCommand, quint8 expectedPayloadLength) const
{
    const auto expectedResponseLength = _overheadLength + expectedCommand.length() + expectedPayloadLength;
    const auto expectedResponseBegin = QByteArray(_preamble).append(_controllerAddress).append(_transceiverAddress);
    const auto expectedBroadcastBegin = QByteArray(_preamble).append(_broadcastAddress).append(_transceiverAddress);

    if (!data.startsWith(expectedResponseBegin))
    {
        if (!data.startsWith(expectedBroadcastBegin))
        {
            return 0;
        }
    }
    if (!data.endsWith(_endOfMessage))
    {
        return 0;
    }
    if (data.length() != expectedResponseLength)
    {
        return 0;
    }
    auto cmd = data.mid(4, expectedCommand.length());
    if (cmd != expectedCommand)
    {
        return 0;
    }
    return 1;
}


QString CIVProtocol::SMeterValueToText(const quint8 value)
{
    // Docu: 120 ... S9, 241 ... S9+60
    auto s = value / 12;
    if (s > 9) s = 9;

    auto sx = QString();
    if (value > 140)
    {
        auto x = ((value - 121) / 20) * 10;
        sx = QString("+%1").arg(x);
    }

    return QString("S%1%2").arg(s).arg(sx);
}


QByteArray CIVProtocol::CreateFrequencyData(const qint32 frequency) const
{
    if (frequency < 0 || frequency > 999999999)
    {
        return QByteArray(5, '\0');
    }

    // Frequency data is BCD coded, however there's a slimmer approach:
    auto freqString = QString().setNum(frequency);
    auto freqBytes = QByteArray().fromHex(freqString.toLatin1());
    std::reverse(freqBytes.begin(), freqBytes.end());

    return freqBytes;
}
