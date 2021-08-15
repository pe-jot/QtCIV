#include <QtTest>
#include "tst_civprotocol_create_test.h"
#include "tst_civprotocol_smeter_test.h"
#include "tst_civprotocol_cmd_test.h"
#include "tst_civprotocol_private_test.h"
#include "tst_civprotocol_parse_test.h"
#include "tst_civprotocol_gpsinfo_test.h"
#include "tst_ic705serial_test.h"


int main(int argc, char *argv[])
{
    TESTLIB_SELFCOVERAGE_START(CIVProtocol_Create_Test)
    TESTLIB_SELFCOVERAGE_START(CIVProtocol_SMeter_Test)
    TESTLIB_SELFCOVERAGE_START(CIVProtocol_Cmd_Test)
    TESTLIB_SELFCOVERAGE_START(CIVProtocol_Private_Test)
    TESTLIB_SELFCOVERAGE_START(CIVProtocol_Parse_Test)
    TESTLIB_SELFCOVERAGE_START(CIVProtocol_GPSInfo_Test)
    TESTLIB_SELFCOVERAGE_START(IC705Serial_Test)

    CIVProtocol_Create_Test tc1;
    CIVProtocol_SMeter_Test tc2;
    CIVProtocol_Cmd_Test tc3;
    CIVProtocol_Private_Test tc4;
    CIVProtocol_Parse_Test tc5;
    CIVProtocol_GPSInfo_Test tc6;
    IC705Serial_Test tc7;

    QTEST_SET_MAIN_SOURCE_PATH

    return    QTest::qExec(&tc1, argc, argv)
            + QTest::qExec(&tc2, argc, argv)
            + QTest::qExec(&tc3, argc, argv)
            + QTest::qExec(&tc4, argc, argv)
            + QTest::qExec(&tc5, argc, argv)
            + QTest::qExec(&tc6, argc, argv)
            + QTest::qExec(&tc7, argc, argv);
}
