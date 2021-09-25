#pragma once
#include <qglobal.h>

class RaspiGpio
{
public:
    RaspiGpio(const quint8& voiceactivityPin, const quint8& heartbeatPin);
    RaspiGpio(const RaspiGpio&) = delete;
    RaspiGpio& operator=(const RaspiGpio&) = delete;

    bool ReadVoiceactivityPin() const;
    bool ReadHeartbeatPin() const;

private:
    bool ReadPin(const quint8& pinNumber) const;

    volatile unsigned *_gpio = nullptr;
    const quint8 _voiceactivityPin;
    const quint8 _heartbeatPin;
};

