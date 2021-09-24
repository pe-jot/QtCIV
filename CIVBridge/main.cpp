#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QLoggingCategory>
#include <QObject>
#include "main.h"
#include "civbridge.h"
#include "consolesignalhandler.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName(APPLICATION_NAME);
    QCoreApplication::setApplicationVersion(APPLICATION_VERSION);

    quint16 websocketPort = DEFAULT_WEBSOCKET_PORT;
    quint16 talkkonnectPort = DEFAULT_TALKKONNECT_PORT;
    quint8 voiceactivityPin = DEFAULT_VOICEACTIVITY_PIN;
    quint8 heartbeatPin = DEFAULT_HEARTBEAT_PIN;

    QCommandLineParser argParser;
    argParser.setApplicationDescription(APPLICATION_DESCRIPTION);
    argParser.addHelpOption(); // -h, --help
    argParser.addVersionOption(); // -v, --version

    QCommandLineOption debugOption({"d", "debug"}, QString("Print debug messages to console"));
    argParser.addOption(debugOption);

    QCommandLineOption websocketPortOption({"w", "websocketport"}, QString("Websocket port to listen on. Default: %1").arg(websocketPort), QString("websocketPort"));
    argParser.addOption(websocketPortOption);

    QCommandLineOption talkkonnectPortOption({"t", "talkkonnectport"}, QString("Talkkonnect HTTP port to listen on. Default: %1").arg(talkkonnectPort), QString("talkkonnectPort"));
    argParser.addOption(talkkonnectPortOption);

    QCommandLineOption voiceactivityPinOption({"a", "voiceactivitypin"}, QString("Raspberry Pi Talkkonnect VoiceActivity Pin number. Default: %1").arg(voiceactivityPin), QString("voiceactivityPin"));
    argParser.addOption(voiceactivityPinOption);

    QCommandLineOption heartbeatPinOption({"b", "heartbeatpin"}, QString("Raspberry Pi Talkkonnect Heartbeat Pin number. Default: %1").arg(heartbeatPin), QString("heartbeatPin"));
    argParser.addOption(heartbeatPinOption);

    argParser.process(a);

    // Enable/disable qDebug messages
    bool debugOutput = argParser.isSet(debugOption);
    QLoggingCategory::setFilterRules(QString("*.debug=%1").arg(debugOutput ? "true" : "false"));

    // Get all integer parameters
    bool conversionOk;
    int intParameter;

    intParameter = argParser.value(websocketPortOption).toInt(&conversionOk);
    if (conversionOk)
    {
        websocketPort = intParameter;
    }

    intParameter = argParser.value(talkkonnectPortOption).toInt(&conversionOk);
    if (conversionOk)
    {
        talkkonnectPort = intParameter;
    }

    intParameter = argParser.value(voiceactivityPinOption).toInt(&conversionOk);
    if (conversionOk)
    {
        voiceactivityPin = intParameter;
    }

    intParameter = argParser.value(heartbeatPinOption).toInt(&conversionOk);
    if (conversionOk)
    {
        heartbeatPin = intParameter;
    }

    // Start CI-V bridge
    auto bridge = new CIVBridge(websocketPort, talkkonnectPort, voiceactivityPin, heartbeatPin);

    // Wrapper to catch CTRL Signals to exit application
    auto consoleSignalHandler = ConsoleSignalHandler::getInstance();
    QObject::connect(consoleSignalHandler, &ConsoleSignalHandler::consoleSignalReceived, bridge, &QObject::deleteLater);
    QObject::connect(consoleSignalHandler, &ConsoleSignalHandler::consoleSignalReceived, &a, &QCoreApplication::quit);

    return a.exec();
}
