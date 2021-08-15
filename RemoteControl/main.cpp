#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "remotecontrol.h"
#include "rotaryencoder.h"
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<RemoteControl>("IC705.RemoteControl", 1, 0, "RemoteControl");
    qmlRegisterType<RotaryEncoder>("IC705.RotaryEncoder", 1, 0, "RotaryEncoder");

    QQmlApplicationEngine engine;

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
