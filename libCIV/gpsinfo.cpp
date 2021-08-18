#include "gpsinfo.h"
#include <QtMath>


inline quint8 BcdByteToDecimal(const quint8& in)
{
    return (in / 16 * 10) + (in % 16);
}


GPSCoordinate::GPSCoordinate()
{
}

void GPSCoordinate::fromRawData(const QByteArray& rawData)
{
    if (rawData.length() == 5)
    {
        _degrees = BcdByteToDecimal(rawData.at(0));
        _minutes = BcdByteToDecimal(rawData.at(1)) + BcdByteToDecimal(rawData.at(2)) * 0.01 + BcdByteToDecimal(rawData.at(3)) * 0.0001;
        _isPositive = (rawData.at(4) == 1); // Northern = positive latitude value
    }
    else if (rawData.length() == 6)
    {
        _degrees = BcdByteToDecimal(rawData.at(0)) * 100 + BcdByteToDecimal(rawData.at(1));
        _minutes = BcdByteToDecimal(rawData.at(2)) + BcdByteToDecimal(rawData.at(3)) * 0.01 + BcdByteToDecimal(rawData.at(4)) * 0.0001;
        _isPositive = (rawData.at(5) == 1); // Eastern = positive longitude value
    }
    else
    {
        return;
    }

    if (!_isPositive)
    {
        _degrees *= -1;
        _minutes *= -1;
    }

    _value = (_degrees + _minutes / 60);
    _isValid = true;
}


void GPSAltitudeInfo::fromRawData(const QByteArray& rawData)
{
    if (rawData.length() != 4)
    {
        return;
    }

    // Alitude may be empty (0xFF) in case of a 2D GPS fix.
    static const auto noAltitude = QByteArray(4, static_cast<unsigned char>(0xFF));
    if (rawData == noAltitude)
    {
        _isValid = false;
        return;
    }

    _altitude = BcdByteToDecimal(rawData.at(0)) * 1000 + BcdByteToDecimal(rawData.at(1)) * 10 + BcdByteToDecimal(rawData.at(2)) * 0.1;
    auto altNegative = rawData.at(3) == 1;
    if (altNegative)
    {
        _altitude *= -1;
    }
    _isValid = true;
}


void GPSCourseInfo::fromRawData(const QByteArray& rawData)
{
    if (rawData.length() != 2)
    {
        return;
    }

    _course = BcdByteToDecimal(rawData.at(0)) * 100 + BcdByteToDecimal(rawData.at(1));
    _isValid = true;
}


void GPSSpeedInfo::fromRawData(const QByteArray& rawData)
{
    if (rawData.length() != 3)
    {
        return;
    }

    // Speed may be empty (0xFF) in case of a 2D GPS fix.
    static const auto noSpeed = QByteArray(3, static_cast<unsigned char>(0xFF));
    if (rawData == noSpeed)
    {
        _isValid = false;
        return;
    }

    _speed = BcdByteToDecimal(rawData.at(0)) * 1000 + BcdByteToDecimal(rawData.at(1)) * 10 + BcdByteToDecimal(rawData.at(2)) * 0.1;
    _isValid = true;
}


void GPSTimeInfo::fromRawData(const QByteArray& rawData)
{
    if (rawData.length() != 7)
    {
        return;
    }

    auto year = BcdByteToDecimal(rawData.at(0)) * 100 + BcdByteToDecimal(rawData.at(1));
    auto month = BcdByteToDecimal(rawData.at(2));
    auto day = BcdByteToDecimal(rawData.at(3));
    auto hour = BcdByteToDecimal(rawData.at(4));
    auto minute = BcdByteToDecimal(rawData.at(5));
    auto second = BcdByteToDecimal(rawData.at(6));

    _dateTime = QDateTime(QDate(year, month, day), QTime(hour, minute, second), Qt::UTC);
    _isValid = _dateTime.isValid();
}


GPSInfo::GPSInfo(const QByteArray& data)
{
    if (data.length() != 27)
    {
        return;
    }

    auto latitude = data.mid(0, 5);
    _latitude.fromRawData(latitude);

    auto longitude = data.mid(5, 6);
    _longitude.fromRawData(longitude);

    auto altitude = data.mid(11, 4);
    _altitude.fromRawData(altitude);

    auto course = data.mid(15, 2);
    _course.fromRawData(course);

    auto speed = data.mid(17, 3);
    _speed.fromRawData(speed);

    auto time = data.mid(20, 7);
    _timestamp.fromRawData(time);
}

bool GPSInfo::isValid() const
{
    return _latitude.isValid() & _longitude.isValid() & _course.isValid() & _speed.isValid() & _timestamp.isValid();
}

QString GPSInfo::getPositionText() const
{
    if (!_latitude.isValid() || !_longitude.isValid())
    {
        return QString("n/a");
    }

    return QString("%1° %2' %3 / %4° %5' %6")
            .arg(abs(_latitude.degrees()))
            .arg(abs(_latitude.minutes()))
            .arg(_latitude.isNorthern() ? 'N' : 'S')
            .arg(abs(_longitude.degrees()))
            .arg(abs(_longitude.minutes()))
            .arg(_longitude.isEastern() ? 'E' : 'W');
}

QString GPSInfo::getMaidenheadLocatorText() const
{
    static const char lowerA = 'a';
    static const char upperA = 'A';
    static const char zero = '0';

    if (!_latitude.isValid() || !_longitude.isValid())
    {
        return QString("n/a");
    }

    // https://de.wikipedia.org/wiki/QTH-Locator
    auto lonDeg = _longitude.degrees() + 180;
    auto lonMin = abs(_longitude.minutes());
    auto latDeg = _latitude.degrees() + 90;
    auto latMin = abs(_latitude.minutes());

    QByteArray qth;

    // Field
    qth.append(upperA + lonDeg / 20); // 1st digit: 20° longitude slot, upper case character A-Z
    qth.append(upperA + latDeg / 10); // 2nd digit: 10° latitude slot, upper case character A-Z

    // Square
    qth.append(zero + (lonDeg % 20) / 2); // 3rd digit: 2° longitude slot, number 0-9
    qth.append(zero + (latDeg % 10) / 1); // 4th digit: 1° latitude slot, number 0-9

    // Subsquare
    qth.append(lowerA + static_cast<int>(lonMin / 5)); // 5th digit: 5' longitude slot, lower case character a-z
    qth.append(lowerA + static_cast<int>(latMin / 2.5)); // 6th digit: 2.5' latitude slot, lower case character a-z

    return QString::fromLatin1(qth);
}
