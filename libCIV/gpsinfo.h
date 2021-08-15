#pragma once
#include <QDateTime>

class GPSCoordinate
{
protected:
    GPSCoordinate();

public:
    GPSCoordinate& operator=(const GPSCoordinate&) = delete;
    GPSCoordinate(const GPSCoordinate&) = delete;

    bool isValid() const { return _isValid; }
    int degrees() const { return _degrees; }
    double minutes() const { return _minutes; }

    void fromRawData(const QByteArray& rawData);

protected:
    bool _isValid = false;
    double _value = 0;
    int _degrees = 0;
    double _minutes = 0;
    bool _isPositive = false;
};


class GPSLatitudeInfo : GPSCoordinate
{
public:
    using GPSCoordinate::fromRawData;
    using GPSCoordinate::isValid;
    using GPSCoordinate::degrees;
    using GPSCoordinate::minutes;
    double latitude() const { return _value; }
    bool isNorthern() const { return _isPositive; }
};


class GPSLongitudeInfo : GPSCoordinate
{
public:
    using GPSCoordinate::fromRawData;
    using GPSCoordinate::isValid;
    using GPSCoordinate::degrees;
    using GPSCoordinate::minutes;
    double longitude() const { return _value; }
    bool isEastern() const { return _isPositive; }
};


class GPSAltitudeInfo
{
public:
    bool isValid() const { return _isValid; }
    double altitude() const { return _altitude; }

    void fromRawData(const QByteArray& rawData);

private:
    bool _isValid = false;
    double _altitude = 0;
};


class GPSCourseInfo
{
public:
    bool isValid() const { return _isValid; }
    int course() const { return _course; }

    void fromRawData(const QByteArray& rawData);

private:
    bool _isValid = false;
    double _course = 0;
};


class GPSSpeedInfo
{
public:
    bool isValid() const { return _isValid; }
    double speed() const { return _speed; }

    void fromRawData(const QByteArray& rawData);

private:
    bool _isValid = false;
    double _speed = 0;
};


class GPSTimeInfo
{
public:
    bool isValid() const { return _isValid; }
    QDateTime dateTime() const { return _dateTime; }

    void fromRawData(const QByteArray& rawData);

private:
    bool _isValid = false;
    QDateTime _dateTime;
};


class GPSInfo
{
public:
    GPSInfo(const QByteArray& rawData = QByteArray());
    GPSInfo(const GPSInfo&) = delete;
    GPSInfo& operator=(const GPSInfo&) = delete;

    bool isValid() const;

    double latitude() { return _latitude.latitude(); }
    double longitude() { return _longitude.longitude(); }
    int course() { return _course.course(); }
    QDateTime timestamp() { return _timestamp.dateTime(); }

    bool isSpeedValid() { return _speed.isValid(); }
    double speed() { return _speed.speed(); }

    bool isAltitudeValid() { return _altitude.isValid(); }
    double altitude() { return _altitude.altitude(); }

    QString getPositionText() const;
    QString getMaidenheadLocatorText() const;

private:
    GPSLatitudeInfo _latitude;
    GPSLongitudeInfo _longitude;
    GPSAltitudeInfo _altitude;
    GPSCourseInfo _course;
    GPSSpeedInfo _speed;
    GPSTimeInfo _timestamp;
};
