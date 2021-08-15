#pragma once
#include <QObject>
#include "civprotocol.h"

class CIVProtocol_Create_Test : public QObject
{
    Q_OBJECT

public:
    CIVProtocol_Create_Test();
    ~CIVProtocol_Create_Test();

private slots:
    void test_CreateCommand();
    void test_CreateCommand_WithData();
    void test_CreateCommand_UserDefinedAddresses();

    void test_CreateResponse();
    void test_CreateResponse_WithData();
    void test_CreateResponse_UserDefinedAddresses();

    void test_ResponseOK();
    void test_ResponseNG();

private:
    CIVProtocol* _civprotocol;
};
