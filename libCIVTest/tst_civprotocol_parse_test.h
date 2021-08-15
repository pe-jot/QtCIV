#pragma once
#include <QObject>
#include "civprotocol.h"

class CIVProtocol_Parse_Test : public QObject
{
    Q_OBJECT

public:
    CIVProtocol_Parse_Test();
    ~CIVProtocol_Parse_Test();

private slots:
    void test_ParseSquelchStatus_data();
    void test_ParseSquelchStatus();

    void test_ParseOvfStatus_data();
    void test_ParseOvfStatus();

    void test_ParseRxFrequencyResponse_data();
    void test_ParseRxFrequencyResponse();

    void test_ParseTxFrequencyResponse_data();
    void test_ParseTxFrequencyResponse();

    void test_ParseRxFrequencyChange_data();
    void test_ParseRxFrequencyChange();

    void test_ParseSMeter_data();
    void test_ParseSMeter();

    void test_ParseGpsStatusResponseData_OK();
    void test_ParseGpsStatusResponseData_NoGpsFix();
    void test_ParseGpsStatusResponseData_InvalidLength();

private:
    CIVProtocol* _civprotocol;
};
