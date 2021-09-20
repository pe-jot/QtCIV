#pragma once

class RaspiGpio
{
public:
    RaspiGpio();
    RaspiGpio(const RaspiGpio&) = delete;
    RaspiGpio& operator=(const RaspiGpio&) = delete;

    bool ReadVoiceActivityPin() const;

private:
    volatile unsigned *gpio = nullptr;
};

