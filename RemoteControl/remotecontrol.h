#pragma once

#include <QQuickItem>
#include <QTimer>
#include "civprotocol.h"
#include "IICOMcomm.h"

class RemoteControl : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString statusText READ getStatusText NOTIFY statusTextChanged)
    Q_PROPERTY(QStringList pollIntervalList READ getPollIntervalList NOTIFY pollIntervalListChanged)
    Q_PROPERTY(int frequencyPollIndex READ getFrequencyPollIndex WRITE setFrequencyPollIndex NOTIFY frequencyPollIndexChanged)

    Q_PROPERTY(int rxFrequency READ getRxFrequency NOTIFY rxFrequencyChanged)
    Q_PROPERTY(int txFrequency READ getTxFrequency NOTIFY txFrequencyChanged)
    Q_PROPERTY(bool squelchStatus READ getSquelchStatus NOTIFY squelchStatusChanged)
    Q_PROPERTY(bool overflowStatus READ getOverflowStatus NOTIFY overflowStatusChanged)
    Q_PROPERTY(float sMeter READ getSMeter NOTIFY sMeterChanged)
    Q_PROPERTY(QString sMeterText READ getSMeterText NOTIFY sMeterTextChanged)

    Q_PROPERTY(bool gpsEnable READ getGpsEnable WRITE setGpsEnable NOTIFY gpsEnableChanged)
    Q_PROPERTY(bool gpsDataValid READ getGpsDataValid NOTIFY gpsDataValidChanged)
    Q_PROPERTY(QString positionText READ getPositionText NOTIFY positionTextChanged)
    Q_PROPERTY(QString locatorText READ getLocatorText NOTIFY locatorTextChanged)
    Q_PROPERTY(bool altitudeValid READ getAltitudeValid NOTIFY altitudeValidChanged)
    Q_PROPERTY(double altitude READ getAltitude NOTIFY altitudeChanged)
    Q_PROPERTY(int heading READ getHeading NOTIFY headingChanged)
    Q_PROPERTY(bool speedValid READ getSpeedValid NOTIFY speedValidChanged)
    Q_PROPERTY(double speed READ getSpeed NOTIFY speedChanged)
    Q_PROPERTY(QDateTime timestamp READ getTimestamp NOTIFY timestampChanged)

public:
    RemoteControl();
    ~RemoteControl();

    QString getStatusText() { return _statusText; }
    void setStatusText(QString statusText, int displayTime = 0)
    {
        if (_statusText != statusText)
        {
            _statusText = statusText;
            emit statusTextChanged();
            if (displayTime > 0)
            {
                _statusTextTimer.singleShot(displayTime, this, SLOT(onStatusTextTimerElapsed()));
            }
        }
    }

    QStringList getPollIntervalList()
    {
        QStringList pollIntervalList;
        foreach (int x, _pollIntervalList)
        {
            pollIntervalList << QString::number(x);
        }
        return pollIntervalList;
    }

    int getFrequencyPollIndex() { return _frequencyPollIndex; }
    void setFrequencyPollIndex(int index)
    {
        if (_frequencyPollIndex != index)
        {
            _frequencyPollIndex = index;
            _pollTimer.setInterval(_pollIntervalList[index]);
            emit frequencyPollIndexChanged();
        }
    }

    int getRxFrequency() { return _rxFrequency; }
    void setRxFrequency(int frequency)
    {
        if (_rxFrequency != frequency)
        {
            _rxFrequency = frequency;
            emit rxFrequencyChanged();
        }
    }

    int getTxFrequency() { return _txFrequency; }
    void setTxFrequency(int frequency)
    {
        if (_txFrequency != frequency)
        {
            _txFrequency = frequency;
            emit txFrequencyChanged();
        }
    }

    bool getSquelchStatus() { return _squelchStatus; }
    void setSquelchStatus(bool squelchStatus)
    {
        if (_squelchStatus != squelchStatus)
        {
            _squelchStatus = squelchStatus;
            emit squelchStatusChanged();
        }
    }

    bool getOverflowStatus() { return _overflowStatus; }
    void setOverflowStatus(bool overflowStatus)
    {
        if (_overflowStatus != overflowStatus)
        {
            _overflowStatus = overflowStatus;
            emit overflowStatusChanged();
        }
    }

    float getSMeter() { return _sMeter; }
    void setSMeter(float sMeter)
    {
        if (_sMeter != sMeter)
        {
            _sMeter = sMeter;
            emit sMeterChanged();
        }
    }

    QString getSMeterText() { return _sMeterText; }
    void setSMeterText(QString sMeterText)
    {
        if (_sMeterText != sMeterText)
        {
            _sMeterText = sMeterText;
            emit sMeterTextChanged();
        }
    }

    bool getGpsEnable() { return _gpsEnable; }
    void setGpsEnable(bool enable)
    {
        if (_gpsEnable != enable)
        {
            _gpsEnable = enable;
            emit gpsEnableChanged();
        }
    }

    bool getGpsDataValid() { return _gpsDataValid; }
    void setGpsDataValid(bool dataValid)
    {
        if (_gpsDataValid != dataValid)
        {
            _gpsDataValid = dataValid;
            emit gpsDataValidChanged();
        }
    }

    QString getPositionText() { return _positionText; }
    void setPositionText(QString positionText)
    {
        if (_positionText != positionText)
        {
            _positionText = positionText;
            emit positionTextChanged();
        }
    }

    QString getLocatorText() { return _locatorText; }
    void setLocatorText(QString locatorText)
    {
        if (_locatorText != locatorText)
        {
            _locatorText = locatorText;
            emit locatorTextChanged();
        }
    }

    bool getAltitudeValid() { return _altitudeValid; }
    void setAltitudeValid(bool altitudeValid)
    {
        if (_altitudeValid != altitudeValid)
        {
            _altitudeValid = altitudeValid;
            emit altitudeValidChanged();
        }
    }

    double getAltitude() { return _altitude; }
    void setAltitude(double altitude)
    {
        if (_altitude != altitude)
        {
            _altitude = altitude;
            emit altitudeChanged();
        }
    }

    bool getSpeedValid() { return _speedValid; }
    void setSpeedValid(bool speedValid)
    {
        if (_speedValid != speedValid)
        {
            _speedValid = speedValid;
            emit speedValidChanged();
        }
    }

    double getSpeed() { return _speed; }
    void setSpeed(double speed)
    {
        if (_speed != speed)
        {
            _speed = speed;
            emit speedChanged();
        }
    }

    int getHeading() { return _heading; }
    void setHeading(int heading)
    {
        if (_heading != heading)
        {
            _heading = heading;
            emit headingChanged();
        }
    }

    QDateTime getTimestamp() { return _timestamp; }
    void setTimeStamp(QDateTime timestamp)
    {
        if (_timestamp != timestamp)
        {
            _timestamp = timestamp;
            emit timestampChanged();
        }
    }

signals:
    void statusTextChanged();
    void pollIntervalListChanged();
    void frequencyPollIndexChanged();
    void rxFrequencyChanged();
    void txFrequencyChanged();
    void squelchStatusChanged();
    void overflowStatusChanged();
    void sMeterChanged();
    void sMeterTextChanged();

    void gpsEnableChanged();
    void gpsDataValidChanged();
    void positionTextChanged();
    void locatorTextChanged();
    void altitudeValidChanged();
    void altitudeChanged();
    void headingChanged();
    void speedValidChanged();
    void speedChanged();
    void timestampChanged();

public slots:
    // Called from QML code, need to be public
    void onVfoWheelIncrement();
    void onVfoWheelDecrement();
    void onVfoStepSizeChanged(const float stepSize);

private slots:
    // Connected inside C++, private is appropriate
    void onGpsEnableChanged();
    void onStatusTextTimerElapsed();

private:
    static const QList<int> _pollIntervalList;

    CIVProtocol _civ;
    QTimer _statusTextTimer;
    QTimer _pollTimer;
    QTimer _gpsTimer;
    IICOMcomm *_comm = nullptr;

    QString _statusText = "";
    int _frequencyPollIndex = 0;
    int _rxFrequency = 0;
    int _txFrequency = 0;
    bool _squelchStatus = 0;
    bool _overflowStatus = 0;
    float _sMeter = 0.0;
    QString _sMeterText = "";
    int _vfoStepSizeHz = 1000;

    bool _gpsEnable = false;
    bool _gpsDataValid = false;
    QString _positionText = "";
    QString _locatorText = "";
    bool _altitudeValid = false;
    double _altitude = 0.0;
    int _heading = 0;
    bool _speedValid = false;
    double _speed = 0.0;
    QDateTime _timestamp;

private:
    void commInitDone(const bool success);
    void dataReceived(const QList<QByteArray> data);
    void pollInterval();
    void gpsPollInterval();
};
