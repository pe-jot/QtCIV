#include <QtTest>
#include "tst_civprotocol_create_test.h"

CIVProtocol_Create_Test::CIVProtocol_Create_Test()
{
    _civprotocol = new CIVProtocol();
}

CIVProtocol_Create_Test::~CIVProtocol_Create_Test()
{
    delete _civprotocol;
}

void CIVProtocol_Create_Test::test_CreateCommand()
{
    // Arrange
    auto cmd = QByteArrayLiteral("\x12\x34\x56");
    auto expectedResult = QByteArrayLiteral("\xFE\xFE\xA4\xE0\x12\x34\x56\xFD");

    // Act
    auto result = _civprotocol->CreateCommand(cmd);

    // Assert
    QCOMPARE(result, expectedResult);
}

void CIVProtocol_Create_Test::test_CreateCommand_WithData()
{
    // Arrange
    auto cmd = QByteArrayLiteral("\x12\x34\x56");
    auto data = QByteArrayLiteral("\xAB\xCD");
    auto expectedResult = QByteArrayLiteral("\xFE\xFE\xA4\xE0\x12\x34\x56\xAB\xCD\xFD");

    // Act
    auto result = _civprotocol->CreateCommand(cmd, data);

    // Assert
    QCOMPARE(result, expectedResult);
}

void CIVProtocol_Create_Test::test_CreateCommand_UserDefinedAddresses()
{
    // Arrange
    auto civprotocol = new CIVProtocol(1, 2);
    auto cmd = QByteArrayLiteral("\x12\x34");
    auto expectedResult = QByteArrayLiteral("\xFE\xFE\x01\x02\x12\x34\xFD");

    // Act
    auto result = civprotocol->CreateCommand(cmd);

    // Assert
    QCOMPARE(result, expectedResult);
}

void CIVProtocol_Create_Test::test_CreateResponse()
{
    // Arrange
    auto cmd = QByteArrayLiteral("\x12\x34\x56");
    auto expectedResult = QByteArrayLiteral("\xFE\xFE\xE0\xA4\x12\x34\x56\xFD");

    // Act
    auto result = _civprotocol->CreateResponse(cmd);

    // Assert
    QCOMPARE(result, expectedResult);
}

void CIVProtocol_Create_Test::test_CreateResponse_WithData()
{
    // Arrange
    auto cmd = QByteArrayLiteral("\x12\x34\x56");
    auto data = QByteArrayLiteral("\xAB\xCD");
    auto expectedResult = QByteArrayLiteral("\xFE\xFE\xE0\xA4\x12\x34\x56\xAB\xCD\xFD");

    // Act
    auto result = _civprotocol->CreateResponse(cmd, data);

    // Assert
    QCOMPARE(result, expectedResult);
}

void CIVProtocol_Create_Test::test_CreateResponse_UserDefinedAddresses()
{
    // Arrange
    auto civprotocol = new CIVProtocol(1, 2);
    auto cmd = QByteArrayLiteral("\x12\x34");
    auto expectedResult = QByteArrayLiteral("\xFE\xFE\x02\x01\x12\x34\xFD");

    // Act
    auto result = civprotocol->CreateResponse(cmd);

    // Assert
    QCOMPARE(result, expectedResult);
}

void CIVProtocol_Create_Test::test_ResponseOK()
{
    // Arrange
    auto expectedResult = QByteArrayLiteral("\xFE\xFE\xE0\xA4\xFB\xFD");

    // Act
    auto result = _civprotocol->ResponseOK;

    // Assert
    QCOMPARE(result, expectedResult);
}

void CIVProtocol_Create_Test::test_ResponseNG()
{
    // Arrange
    auto expectedResult = QByteArrayLiteral("\xFE\xFE\xE0\xA4\xFA\xFD");

    // Act
    auto result = _civprotocol->ResponseNG;

    // Assert
    QCOMPARE(result, expectedResult);
}
