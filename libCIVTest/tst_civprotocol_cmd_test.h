#pragma once
#include <QObject>
#include "civprotocol.h"

class CIVProtocol_Cmd_Test : public QObject
{
    Q_OBJECT

public:
    CIVProtocol_Cmd_Test();
    ~CIVProtocol_Cmd_Test();

private slots:
    void test_CmdReadTransceiverId();
    void test_ResponseReadTransceiverId();

    void test_CmdReadSquelchStatus();
    void test_CmdReadOvfStatus();
    void test_CmdReadSMeter();
    void test_CmdReadRxFreq();
    void test_CmdReadTxFreq();
    void test_CmdWriteVfoFrequency_SelectedVFO();
    void test_CmdWriteVfoFrequency_UnselectedVFO();
    void test_CmdSetGpsPower_ON();
    void test_CmdSetGpsPower_OFF();
    void test_CmdGetGpsStatus();

private:
    CIVProtocol* _civprotocol;
};
