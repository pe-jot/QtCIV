#include "satsync.h"
#include "ic705serial.h"
#include <QDebug>

SatSync::SatSync()
{
    _comm = new IC705Serial();
}


SatSync::~SatSync()
{
    if (_comm != nullptr)
    {
        disconnect(_comm, &IICOMcomm::dataReceived, this, &SatSync::dataReceived);
        delete _comm;
        _comm = nullptr;
    }
}


void SatSync::init()
{
    connect(_comm, &IICOMcomm::initComplete, this, &SatSync::commInitDone);
    _comm->init();
}


void SatSync::startSync()
{
    _sync = true;
    connect(this, &SatSync::rxFrequencyChanged, this, &SatSync::syncTxFrequency);
}


void SatSync::stopSync()
{
    _sync = false;
    disconnect(this, &SatSync::rxFrequencyChanged, this, &SatSync::syncTxFrequency);
}


void SatSync::commInitDone(const bool success)
{
    disconnect(_comm, &IICOMcomm::initComplete, this, &SatSync::commInitDone);

    if (!success)
    {
        emit initDone(false, QString("Error: IC705 not found!"));
        return;
    }

    emit initDone(true, QString("Communication established on %1.").arg(_comm->getName()));

    connect(_comm, &IICOMcomm::dataReceived, this, &SatSync::dataReceived);
    _comm->writeData(_civ.CmdReadRxFreq());
    _comm->writeData(_civ.CmdReadTxFreq());
}


void SatSync::dataReceived(const QList<QByteArray> data)
{
    for (auto it = data.constBegin(); it != data.constEnd(); ++it)
    {
        auto rxFrequency = _civ.ParseRxFrequencyResponse(*it);
        if (rxFrequency != -1)
        {
            setRxFrequency(rxFrequency);
            continue;
        }

        rxFrequency = _civ.ParseRxFrequencyChange(*it);
        if (rxFrequency != -1)
        {
            setRxFrequency(rxFrequency);
            if (!_sync)
            {
                _comm->writeData(_civ.CmdReadTxFreq());
            }
            continue;
        }

        auto txFrequency = _civ.ParseTxFrequencyResponse(*it);
        if (txFrequency != -1)
        {
            setTxFrequency(txFrequency);
            continue;
        }

        if (*it == _civ.ResponseOK)
        {
            continue;
        }

        qDebug().nospace() << "Unknown data received (" << (*it).length() << " Bytes): " << *it;
    }
}


void SatSync::syncTxFrequency()
{
    // xf = fTx / fRx
    // dfRx = fRx - fRx0
    // dfTx = dfRx * xf = (fRx - fRx0) * (fTx / fRx)
    // fTx = fTx0 - dfTx = fTx0 - (fRx - fRx0) * (fTx / fRx)

    auto deltaRxFrequency = _rxFrequency - _nominalRxFrequency;
    auto deltaTxFrequency = deltaRxFrequency * (double)_nominalTxFrequency / (double)_nominalRxFrequency;
    auto newTxFrequency = _nominalTxFrequency - (int)deltaTxFrequency;

    _comm->writeData(_civ.CmdWriteVfoFrequency(newTxFrequency, CIVProtocol::VFO::Unselected));
    _comm->writeData(_civ.CmdReadTxFreq());
}
