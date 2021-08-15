#pragma once
#include <QObject>
#include "civprotocol.h"

class CIVProtocol_SMeter_Test : public QObject
{
    Q_OBJECT

public:
    CIVProtocol_SMeter_Test();
    ~CIVProtocol_SMeter_Test();

private slots:
    void test_SMeterValueToText_data();
    void test_SMeterValueToText();

private:
    CIVProtocol* _civprotocol;
};
