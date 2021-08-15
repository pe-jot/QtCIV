#include <QtTest>
#include "tst_civprotocol_smeter_test.h"

CIVProtocol_SMeter_Test::CIVProtocol_SMeter_Test()
{
    _civprotocol = new CIVProtocol();
}

CIVProtocol_SMeter_Test::~CIVProtocol_SMeter_Test()
{
    delete _civprotocol;
}

void CIVProtocol_SMeter_Test::test_SMeterValueToText_data()
{
    QTest::addColumn<int>("value");
    QTest::addColumn<QString>("expectedResult");

    QTest::newRow("S0") << 0 << "S0";
    QTest::newRow("S9") << 120 << "S9";
    QTest::newRow("S9+60") << 241 << "S9+60";
}

void CIVProtocol_SMeter_Test::test_SMeterValueToText()
{
    // Arrange
    QFETCH(int, value);
    QFETCH(QString, expectedResult);

    // Act
    auto result = CIVProtocol::SMeterValueToText(value);

    // Assert
    QCOMPARE(result, expectedResult);
}
