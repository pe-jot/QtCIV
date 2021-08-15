#pragma once

#include <QObject>
#include <QTimer>
#include "civprotocol.h"
#include "IICOMcomm.h"

class SatSync : public QObject
{
    Q_OBJECT

public:
    SatSync();
    ~SatSync();
    void init();
    void startSync();
    void stopSync();

    int getRxFrequency() { return _rxFrequency; }
    void setRxFrequency(int frequency)
    {
        if (_rxFrequency != frequency)
        {
            _rxFrequency = frequency;
            if (_nominalRxFrequency == -1)
            {
                setNominalRxFrequency(frequency);
            }
            emit rxFrequencyChanged();
        }
    }

    int getTxFrequency() { return _txFrequency; }
    void setTxFrequency(int frequency)
    {
        if (_txFrequency != frequency)
        {
            _txFrequency = frequency;
            if (_nominalTxFrequency == -1)
            {
                setNominalTxFrequency(frequency);
            }
            emit txFrequencyChanged();
        }
    }

    int getNominalRxFrequency() { return _nominalRxFrequency; }
    void setNominalRxFrequency(int frequency)
    {
        if (_nominalRxFrequency != frequency)
        {
            _nominalRxFrequency = frequency;
            emit nominalRxFrequencyChanged();
        }
    }

    int getNominalTxFrequency() { return _nominalTxFrequency; }
    void setNominalTxFrequency(int frequency)
    {
        if (_nominalTxFrequency != frequency)
        {
            _nominalTxFrequency = frequency;
            emit nominalTxFrequencyChanged();
        }
    }

signals:
    void initDone(const bool success, const QString message);
    void rxFrequencyChanged();
    void txFrequencyChanged();
    void nominalRxFrequencyChanged();
    void nominalTxFrequencyChanged();

private:
    CIVProtocol _civ;
    IICOMcomm *_comm = nullptr;

    bool _sync = 0;
    int _rxFrequency = 0;
    int _txFrequency = 0;
    int _nominalRxFrequency = -1;
    int _nominalTxFrequency = -1;

private:
    void dataReceived(const QList<QByteArray> data);
    void syncTxFrequency();
    void commInitDone(const bool success);
};

