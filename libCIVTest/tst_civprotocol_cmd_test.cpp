#include <QtTest>
#include "tst_civprotocol_cmd_test.h"

CIVProtocol_Cmd_Test::CIVProtocol_Cmd_Test()
{
    _civprotocol = new CIVProtocol();
}

CIVProtocol_Cmd_Test::~CIVProtocol_Cmd_Test()
{
    delete _civprotocol;
}

void CIVProtocol_Cmd_Test::test_CmdReadTransceiverId()
{
    // Arrange
    auto expectedResult = QByteArrayLiteral("\xFE\xFE\xA4\xE0\x19\x00\xFD");

    // Act
    auto result = _civprotocol->CmdReadTransceiverId();

    // Assert
    QCOMPARE(result, expectedResult);
}

void CIVProtocol_Cmd_Test::test_ResponseReadTransceiverId()
{
    // Arrange
    auto expectedResult = QByteArrayLiteral("\xFE\xFE\xE0\xA4\x19\x00\xA4\xFD");

    // Act
    auto result = _civprotocol->ResponseReadTransceiverId();

    // Assert
    QCOMPARE(result, expectedResult);
}

void CIVProtocol_Cmd_Test::test_CmdReadSquelchStatus()
{
    // Arrange
    auto expectedResult = QByteArrayLiteral("\xFE\xFE\xA4\xE0\x15\x01\xFD");

    // Act
    auto result = _civprotocol->CmdReadSquelchStatus();

    // Assert
    QCOMPARE(result, expectedResult);
}

void CIVProtocol_Cmd_Test::test_CmdReadOvfStatus()
{
    // Arrange
    auto expectedResult = QByteArrayLiteral("\xFE\xFE\xA4\xE0\x15\x07\xFD");

    // Act
    auto result = _civprotocol->CmdReadOvfStatus();

    // Assert
    QCOMPARE(result, expectedResult);
}

void CIVProtocol_Cmd_Test::test_CmdReadSMeter()
{
    // Arrange
    auto expectedResult = QByteArrayLiteral("\xFE\xFE\xA4\xE0\x15\x02\xFD");

    // Act
    auto result = _civprotocol->CmdReadSMeter();

    // Assert
    QCOMPARE(result, expectedResult);
}

void CIVProtocol_Cmd_Test::test_CmdReadRxFreq()
{
    // Arrange
    auto expectedResult = QByteArrayLiteral("\xFE\xFE\xA4\xE0\x03\xFD");

    // Act
    auto result = _civprotocol->CmdReadRxFreq();

    // Assert
    QCOMPARE(result, expectedResult);
}

void CIVProtocol_Cmd_Test::test_CmdReadTxFreq()
{
    // Arrange
    auto expectedResult = QByteArrayLiteral("\xFE\xFE\xA4\xE0\x1C\x03\xFD");

    // Act
    auto result = _civprotocol->CmdReadTxFreq();

    // Assert
    QCOMPARE(result, expectedResult);
}

void CIVProtocol_Cmd_Test::test_CmdWriteVfoFrequency_SelectedVFO()
{
    // Arrange
    auto expectedResult = QByteArrayLiteral("\xFE\xFE\xA4\xE0\x25\x00\x89\x67\x45\x23\x01\xFD");

    // Act
    auto result = _civprotocol->CmdWriteVfoFrequency(123456789, CIVProtocol::VFO::Selected);

    // Assert
    QCOMPARE(result, expectedResult);
}

void CIVProtocol_Cmd_Test::test_CmdWriteVfoFrequency_UnselectedVFO()
{
    // Arrange
    auto expectedResult = QByteArrayLiteral("\xFE\xFE\xA4\xE0\x25\x01\x89\x67\x45\x23\x01\xFD");

    // Act
    auto result = _civprotocol->CmdWriteVfoFrequency(123456789, CIVProtocol::VFO::Unselected);

    // Assert
    QCOMPARE(result, expectedResult);
}

void CIVProtocol_Cmd_Test::test_CmdSetGpsPower_ON()
{
    // Arrange
    auto expectedResult = QByteArrayLiteral("\xFE\xFE\xA4\xE0\x23\x01\x01\xFD");

    // Act
    auto result = _civprotocol->CmdSetGpsPower(true);

    // Assert
    QCOMPARE(result, expectedResult);
}

void CIVProtocol_Cmd_Test::test_CmdSetGpsPower_OFF()
{
    // Arrange
    auto expectedResult = QByteArrayLiteral("\xFE\xFE\xA4\xE0\x23\x01\x00\xFD");

    // Act
    auto result = _civprotocol->CmdSetGpsPower(false);

    // Assert
    QCOMPARE(result, expectedResult);
}

void CIVProtocol_Cmd_Test::test_CmdGetGpsStatus()
{
    // Arrange
    auto expectedResult = QByteArrayLiteral("\xFE\xFE\xA4\xE0\x23\x00\xFD");

    // Act
    auto result = _civprotocol->CmdGetGpsStatus();

    // Assert
    QCOMPARE(result, expectedResult);
}
