#include <QCoreApplication>
#include <QObject>
#include "civbridge.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    CIVBridge bridge;
    return a.exec();
}
