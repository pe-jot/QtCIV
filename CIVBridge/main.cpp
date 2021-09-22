#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QLoggingCategory>
#include <QObject>
#include "civbridge.h"
#include "consolesignalhandler.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("CI-V Bridge");
    QCoreApplication::setApplicationVersion("1.0");

    quint16 websocketPort = 87;
    quint16 talkkonnectPort = 81;

    QCommandLineParser argParser;
    argParser.setApplicationDescription("Receives messages on a UDP port and prints them to stdout.");
    argParser.addHelpOption();
    argParser.addVersionOption();

    QCommandLineOption debugOption({"d", "debug"}, QString("Print debug messages to console"), QString("debug"));
    argParser.addOption(debugOption);

    QCommandLineOption websocketPortOption({"w", "websocketport"}, QString("Websocket port to listen on. Default: %1").arg(websocketPort), QString("websocketport"));
    argParser.addOption(websocketPortOption);

    QCommandLineOption talkkonnectPortOption({"t", "talkkonnectport"}, QString("Talkkonnect HTTP port to listen on. Default: %1").arg(talkkonnectPort), QString("talkkonnectport"));
    argParser.addOption(websocketPortOption);

    argParser.process(a);

    // Enable/disable qDebug messages
    bool debugOutput = argParser.isSet(debugOption);
    QLoggingCategory::setFilterRules(QString("*.debug=%1").arg(debugOutput ? "true" : "false"));

    // Get Websocket port parameter
    auto conversionOk = false;
    auto conversionResult = argParser.value(websocketPortOption).toInt(&conversionOk);
    if (conversionOk)
    {
        websocketPort = conversionResult;
    }

    // Get Talkkonnect HTTP port parameter
    conversionResult = argParser.value(talkkonnectPortOption).toInt(&conversionOk);
    if (conversionOk)
    {
        talkkonnectPort = conversionResult;
    }

    // Start CI-V bridge
    auto bridge = new CIVBridge(websocketPort, talkkonnectPort);

    // Wrapper to catch CTRL Signals to exit application
    auto consoleSignalHandler = ConsoleSignalHandler::getInstance();
    QObject::connect(consoleSignalHandler, &ConsoleSignalHandler::consoleSignalReceived, bridge, &QObject::deleteLater);
    QObject::connect(consoleSignalHandler, &ConsoleSignalHandler::consoleSignalReceived, &a, &QCoreApplication::quit);

    return a.exec();
}
