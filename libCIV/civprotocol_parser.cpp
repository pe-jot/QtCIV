#include "civprotocol.h"
#include <QString>


inline quint8 BcdByteToDecimal(const quint8& in)
{
    return (in / 16 * 10) + (in % 16);
}


qint8 CIVProtocol::ParseSquelchStatus(const QByteArray& data) const
{
    const auto payloadLength = 1;
    const auto payloadOffset = 6;

    if (!CheckResponse(data, _cmdReadSquelchStatus, payloadLength))
    {
        return -1;
    }

    auto squelchStatus = data.at(payloadOffset);
    return squelchStatus;
}


qint8 CIVProtocol::ParseOvfStatus(const QByteArray& data) const
{
    const auto payloadLength = 1;
    const auto payloadOffset = 6;

    if (!CheckResponse(data, _cmdReadOvfStatus, payloadLength))
    {
        return -1;
    }

    auto squelchStatus = data.at(payloadOffset);
    return squelchStatus;
}


qint32 CIVProtocol::ParseSMeter(const QByteArray& data) const
{
    const auto payloadLength = 2;
    const auto payloadOffset = 6;

    if (!CheckResponse(data, _cmdReadSMeter, payloadLength))
    {
        return -1;
    }

    auto sMeter = data.mid(payloadOffset, payloadLength);

    return BcdByteToDecimal(sMeter.at(0)) * 100 + BcdByteToDecimal(sMeter.at(1));
}


GPSInfo* CIVProtocol::ParseGpsStatusResponseData(const QByteArray &data) const
{
    const auto payloadLength = 27;
    const auto payloadOffset = 6;

    if (!CheckResponse(data, _cmdGetGpsStatus, payloadLength))
    {
        if (CheckResponse(data, _cmdGetGpsStatus, 1))
        {
            if (data.mid(payloadOffset, 1) == QByteArrayLiteral("\xFF"))
            {
                return new GPSInfo();
            }
        }
        return nullptr;
    }

    auto payload = data.mid(payloadOffset, payloadLength);
    // Lat: "48 09 40 50 01" Lon: "00 14 00 45 90 01" Alt: "00 30 16 00" Cse: "00 50" Spd: "00 00 11" Tme: "20 21 08 08 13 56 16"

    return new GPSInfo(payload);
}


qint32 CIVProtocol::ParseRxFrequencyResponse(const QByteArray& data) const
{
    const auto payloadLength = 5;
    const auto payloadOffset = 5;

    if (!CheckResponse(data, _cmdReadRxFrequency, payloadLength))
    {
        return -1;
    }

    auto frequencyData = data.mid(payloadOffset, payloadLength);
    return ParseFrequencyData(frequencyData);
}


qint32 CIVProtocol::ParseTxFrequencyResponse(const QByteArray& data) const
{
    const auto payloadLength = 5;
    const auto payloadOffset = 6;

    if (!CheckResponse(data, _cmdReadTxFrequency, payloadLength))
    {
        return -1;
    }

    auto frequencyData = data.mid(payloadOffset, payloadLength);
    return ParseFrequencyData(frequencyData);
}


qint32 CIVProtocol::ParseRxFrequencyChange(const QByteArray &data) const
{
    const auto payloadLength = 5;
    const auto payloadOffset = 5;

    if (!CheckResponse(data, _cmdRxFrequencyChange, payloadLength))
    {
        return -1;
    }

    auto frequencyData = data.mid(payloadOffset, payloadLength);
    return ParseFrequencyData(frequencyData);
}


qint32 CIVProtocol::ParseFrequencyData(const QByteArray& data) const
{
    if (data.length() != 5)
    {
        return -1;
    }

    // Frequency data is BCD coded, however there's a slimmer approach:
    // Make a copy because reverse() directly modifies the data
    auto temp = QByteArray(data);

    // Frequency data comes in reverese order
    std::reverse(temp.begin(), temp.end());

    // toHex() does the conversion trick, then simply make an int out of the string
    return temp.toHex().toInt();
}
