#include "consolesignalhandler.h"
#include <QMutexLocker>
#include <QDebug>

ConsoleSignalHandler* ConsoleSignalHandler::inst = nullptr;
QMutex ConsoleSignalHandler::ctorMutex;


#if defined(_WIN32) || defined(_WIN64)
BOOL WINAPI ConsoleSignalHandler::ctrlHandler(DWORD fdwCtrlType)
{
    switch (fdwCtrlType)
    {
    // See https://docs.microsoft.com/en-us/windows/console/handlerroutine
    case CTRL_C_EVENT:
    case CTRL_BREAK_EVENT:
    case CTRL_CLOSE_EVENT:
    case CTRL_SHUTDOWN_EVENT:
        if (inst != nullptr)
        {
            emit inst->consoleSignalReceived();
        }
        return true;

    default:
        return false;
    }
}
#else
void ConsoleSignalHandler::signalHandler(int signum)
{
    Q_UNUSED(signum)
    if (inst != nullptr)
    {
        emit inst->consoleSignalReceived();
    }
}
#endif


ConsoleSignalHandler* ConsoleSignalHandler::getInstance()
{
    QMutexLocker ctorLocker(&ctorMutex);

    if (inst == nullptr)
    {
        inst = new ConsoleSignalHandler();
#if defined(_WIN32) || defined(_WIN64)
        if (!SetConsoleCtrlHandler(ctrlHandler, true))
        {
            qCritical() << "SetConsoleCtrlHandler() failed!";
        }
#else
        ::signal(SIGINT, signalHandler);
        ::signal(SIGTERM, signalHandler);
        ::signal(SIGHUP, signalHandler);
#endif
    }

    return inst;
}
