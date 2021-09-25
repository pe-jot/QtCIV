#include "raspigpio.h"
#include <QtGlobal>
#include <QDebug>

#ifdef Q_OS_LINUX

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <error.h>

#define RPI1_PERI_BASE		0x20000000						// RPi 1
#define BCM2708_PERI_BASE	0x3F000000						// RPi 2 & 3
#define BCM2711_PERI_BASE	0xFE000000						// RPi 4
#define GPIO_BASE			(BCM2711_PERI_BASE + 0x200000)	// GPIO controller
#define PAGE_SIZE			(4 * 1024)
#define BLOCK_SIZE			(4 * 1024)

#define GET_GPIO(g)         (*(_gpio + 13) & (1 << g))       // 0 if LOW, (1 << g) if HIGH

RaspiGpio::RaspiGpio(const quint8& voiceactivityPin, const quint8& heartbeatPin)
    : _voiceactivityPin(voiceactivityPin)
    , _heartbeatPin(heartbeatPin)
{
    // Set up memory regions to access GPIO
    int  mem_fd;
    void *gpio_map;

    // open /dev/mem
    if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0)
    {
        qCritical() << "Error: can't open /dev/mem";
        exit(-1);
    }

    // mmap GPIO
    gpio_map = mmap(
        NULL,						// Any adddress in our space will do
        BLOCK_SIZE,					// Map length
        PROT_READ | PROT_WRITE,		// Enable reading & writting to mapped memory
        MAP_SHARED,					// Shared with other processes
        mem_fd,						// File to map
        GPIO_BASE					// Offset to GPIO peripheral
    );

    close(mem_fd); // No need to keep mem_fd open after mmap

    if (gpio_map == MAP_FAILED)
    {
        qCritical() << "mmap error" << errno;
        return;
    }

    // Always use volatile pointer!
    _gpio = (volatile unsigned *)gpio_map;
}

bool RaspiGpio::ReadPin(const quint8& pinNumber) const
{
    if (_gpio == nullptr)
    {
        return false;
    }
    return (GET_GPIO(pinNumber) != 0);
}

bool RaspiGpio::ReadVoiceactivityPin() const
{
    return ReadPin(_voiceactivityPin);
}

bool RaspiGpio::ReadHeartbeatPin() const
{
    return ReadPin(_heartbeatPin);
}

#else

RaspiGpio::RaspiGpio(const quint8& voiceactivityPin, const quint8& heartbeatPin)
    : _voiceactivityPin(voiceactivityPin)
    , _heartbeatPin(heartbeatPin)
{
}

bool RaspiGpio::ReadVoiceactivityPin() const
{
    return false;
}

bool RaspiGpio::ReadHeartbeatPin() const
{
    // Simulate toggling heartbeat pin
    static bool pinState = false;
    pinState = !pinState;
    return pinState;
}

#endif
