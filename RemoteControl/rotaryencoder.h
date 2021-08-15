#pragma once

#include <QObject>

class RotaryEncoder : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int max READ getMax NOTIFY maxChanged)
    Q_PROPERTY(int value READ getValue WRITE setValue)
    Q_PROPERTY(QStringList stepSizeList READ getStepSizeList NOTIFY stepSizeListChanged)
    Q_PROPERTY(int stepSizeIndex READ getStepSizeIndex WRITE setStepSizeIndex NOTIFY stepSizeIndexChanged)

public:
    RotaryEncoder();

    int getMax() { return _max; }

    int getValue() { return _value; }
    void setValue(const int value)
    {
        if (value != _value)
        {
            if ((value > _value) || (_value == _max && value == _min))
            {
                emit increment();
            }
            if ((value < _value) || (_value == _min && value == _max))
            {
                emit decrement();
            }
            _value = value;
        }
    }

    QStringList getStepSizeList()
    {
        QStringList pollIntervalList;
        foreach (float x, _stepSizeList)
        {
            pollIntervalList << QString::number(x, 'f', 1).append(" kHz");
        }
        return pollIntervalList;
    }

    int getStepSizeIndex() { return _stepSizeIndex; }
    void setStepSizeIndex(int index)
    {
        if (_stepSizeIndex != index)
        {
            _stepSizeIndex = index;
            emit stepSizeIndexChanged();
            emit stepSizeChanged(_stepSizeList[_stepSizeIndex]);
        }
    }

    float getStepSize() { return _stepSizeList[_stepSizeIndex]; }

signals:
    void maxChanged();
    void increment();
    void decrement();
    void stepSizeListChanged();
    void stepSizeIndexChanged();
    void stepSizeChanged(const float value);

private:
    const int _min = 0;
    int _max = 100;
    int _value = 0;
    static const QList<float> _stepSizeList;
    int _stepSizeIndex = 0;
};
