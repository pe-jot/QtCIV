#include "remotecontrol.h"
#include "ic705serial.h"
#include <QDebug>


const QList<int> RemoteControl::_pollIntervalList = QList<int>() << 100 << 250 << 500 << 1000;


RemoteControl::RemoteControl()
{
    _comm = new IC705Serial();
    connect(_comm, &IICOMcomm::initComplete, this, &RemoteControl::commInitDone);
    _comm->init();
}


RemoteControl::~RemoteControl()
{
    if (_comm != nullptr)
    {
        QObject::disconnect(_comm, &IICOMcomm::dataReceived, this, &RemoteControl::dataReceived);
        delete _comm;
        _comm = nullptr;
    }
}


void RemoteControl::commInitDone(const bool success)
{
    disconnect(_comm, &IICOMcomm::initComplete, this, &RemoteControl::commInitDone);
    if (!success)
    {
        return;
    }

    setStatusText(QString("Communication established on %1.").arg(_comm->getName()), 3000);

    connect(_comm, &IICOMcomm::dataReceived, this, &RemoteControl::dataReceived);

    _pollTimer.setInterval(_pollIntervalList[_frequencyPollIndex]);
    connect(&_pollTimer, &QTimer::timeout, this, &RemoteControl::pollInterval);
    _pollTimer.start();

    _gpsTimer.setInterval(1000);
    connect(&_gpsTimer, &QTimer::timeout, this, &RemoteControl::gpsPollInterval);
    connect(this, &RemoteControl::gpsEnableChanged, this, &RemoteControl::onGpsEnableChanged);
}


void RemoteControl::onStatusTextTimerElapsed()
{
    setStatusText("");
}


void RemoteControl::pollInterval()
{
    _comm->writeData(_civ.CmdReadRxFreq());
    _comm->writeData(_civ.CmdReadTxFreq());
    _comm->writeData(_civ.CmdReadSquelchStatus());
    _comm->writeData(_civ.CmdReadOvfStatus());
    _comm->writeData(_civ.CmdReadSMeter());
}


void RemoteControl::onGpsEnableChanged()
{
    _gpsEnable ? _gpsTimer.start() : _gpsTimer.stop();
    _comm->writeData(_civ.CmdSetGpsPower(_gpsEnable));
}


void RemoteControl::gpsPollInterval()
{
    _comm->writeData(_civ.CmdGetGpsStatus());
}


void RemoteControl::dataReceived(const QList<QByteArray> data)
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
            setSMeter(sMeter / 255.0); // S-Meter on GUI is from 0.0 - 1.0
            setSMeterText(CIVProtocol::SMeterValueToText(sMeter));
            continue;
        }

        auto gps = _civ.ParseGpsStatusResponseData(*it);
        if (gps != nullptr)
        {
            setGpsDataValid(gps->isValid());
            if (gps->isValid())
            {
                setPositionText(gps->getPositionText());
                setLocatorText(gps->getMaidenheadLocatorText());
                setAltitudeValid(gps->isAltitudeValid());
                setAltitude(gps->altitude());
                setHeading(gps->course());
                setSpeedValid(gps->isSpeedValid());
                setSpeed(gps->speed());
                setTimeStamp(gps->timestamp());
            }
            delete gps;
            continue;
        }

        if (*it == _civ.ResponseOK)
        {
            continue;
        }

        qDebug().nospace() << "Unknown data received (" << (*it).length() << " Bytes): " << *it;
    }
}


void RemoteControl::onVfoWheelIncrement()
{
    _comm->writeData(_civ.CmdWriteVfoFrequency(_rxFrequency + _vfoStepSizeHz, CIVProtocol::VFO::Selected));
    _comm->writeData(_civ.CmdReadRxFreq());
}


void RemoteControl::onVfoWheelDecrement()
{
    _comm->writeData(_civ.CmdWriteVfoFrequency(_rxFrequency - _vfoStepSizeHz, CIVProtocol::VFO::Selected));
    _comm->writeData(_civ.CmdReadRxFreq());
}


void RemoteControl::onVfoStepSizeChanged(const float stepSize)
{
    _vfoStepSizeHz = static_cast<int>(stepSize * 1000);
}
