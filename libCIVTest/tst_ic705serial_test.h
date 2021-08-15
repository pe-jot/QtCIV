#pragma once
#include <QObject>
#include "ic705serial.h"

class IC705Serial_Test : public QObject
{
    Q_OBJECT

public:
    IC705Serial_Test();
    ~IC705Serial_Test();

private slots:
    void test_SplitReceivedMessages_data();
    void test_SplitReceivedMessages();

private:
    IC705Serial* _ic705serial;
};
