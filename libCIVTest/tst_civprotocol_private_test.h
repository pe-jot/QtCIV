#pragma once
#include <QObject>
#include "civprotocol.h"

class CIVProtocol_Private_Test : public QObject
{
    Q_OBJECT

public:
    CIVProtocol_Private_Test();
    ~CIVProtocol_Private_Test();

private slots:
    void test_TransceiverAddress();
    void test_TransceiverAddressDefaultValue();
    void test_ControllerAddress();
    void test_ControllerAddressDefaultValue();
    void test_BroadcastAddress();
    void test_OverheadLength();

    void test_CheckResponse_data();
    void test_CheckResponse();

    void test_CreateFrequencyData_data();
    void test_CreateFrequencyData();

    void test_ParseFrequencyData_data();
    void test_ParseFrequencyData();

private:
    CIVProtocol* _civprotocol;
};
