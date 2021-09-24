#pragma once

#include <QMutex>
#include <QObject>
#include <signal.h>

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#endif

class ConsoleSignalHandler : public QObject
{
    Q_OBJECT

public:
    static ConsoleSignalHandler* getInstance();

signals:
    void consoleSignalReceived();

private:
    ConsoleSignalHandler() = default;
    ConsoleSignalHandler(const ConsoleSignalHandler&) = delete;
    ConsoleSignalHandler& operator=(const ConsoleSignalHandler&) = delete;

#if defined(_WIN32) || defined(_WIN64)
    static BOOL WINAPI ctrlHandler(DWORD fdwCtrlType);
#else
    static void signalHandler(int signum);
#endif

    static ConsoleSignalHandler* inst;
    static QMutex ctorMutex;
};

