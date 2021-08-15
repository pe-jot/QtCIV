#include <QtTest>
#include "tst_civprotocol_private_test.h"

CIVProtocol_Private_Test::CIVProtocol_Private_Test()
{
    _civprotocol = new CIVProtocol();
}

CIVProtocol_Private_Test::~CIVProtocol_Private_Test()
{
    delete _civprotocol;
}

void CIVProtocol_Private_Test::test_TransceiverAddress()
{
    // Arrange
    auto expected = 0x12;

    // Act
    CIVProtocol civprotocol(expected, 0x34);

    // Assert
    QCOMPARE(civprotocol._transceiverAddress.at(0), expected);
}

void CIVProtocol_Private_Test::test_TransceiverAddressDefaultValue()
{
    // Assert
    QCOMPARE(static_cast<quint8>(_civprotocol->_transceiverAddress.at(0)), 0xA4);
}

void CIVProtocol_Private_Test::test_ControllerAddress()
{
    // Arrange
    auto expected = 0x34;

    // Act
    CIVProtocol civprotocol(0x12, expected);

    // Assert
    QCOMPARE(civprotocol._controllerAddress.at(0), expected);
}

void CIVProtocol_Private_Test::test_ControllerAddressDefaultValue()
{
    // Assert
    QCOMPARE(static_cast<quint8>(_civprotocol->_controllerAddress.at(0)), 0xE0);
}

void CIVProtocol_Private_Test::test_BroadcastAddress()
{
    // Assert
    QCOMPARE(_civprotocol->_broadcastAddress.at(0), 0x00);
}

void CIVProtocol_Private_Test::test_OverheadLength()
{
    // Assert
    QCOMPARE(_civprotocol->_overheadLength, 5);
}

void CIVProtocol_Private_Test::test_CheckResponse_data()
{
    QTest::addColumn<QByteArray>("data");
    QTest::addColumn<QByteArray>("command");
    QTest::addColumn<int>("payloadLength");
    QTest::addColumn<bool>("expectedResult");

    QTest::newRow("ResponseOK") << QByteArrayLiteral("\xFE\xFE\xE0\xA4\x19\x00\xA4\xFD") << QByteArrayLiteral("\x19\x00") << 1 << true;
    QTest::newRow("BroadcastResponseOK") << QByteArrayLiteral("\xFE\xFE\x00\xA4\x00\x12\x34\x56\x78\x90\xFD") << QByteArrayLiteral("\x00") << 5 << true;
    QTest::newRow("InvalidStart") << QByteArrayLiteral("\xFE\xFF\xE0\xA4\x19\x00\xA4\xFD") << QByteArrayLiteral("\x19\x00") << 1 << false;
    QTest::newRow("InvalidEnd") << QByteArrayLiteral("\xFE\xFE\xE0\xA4\x19\x00\xA4\xFF") << QByteArrayLiteral("\x19\x00") << 1 << false;
    QTest::newRow("InvalidLength") << QByteArrayLiteral("\xFE\xFE\xE0\xA4\x19\x00\xA4\xA5\xFD") << QByteArrayLiteral("\x19\x00") << 1 << false;
    QTest::newRow("InvalidExpectedCommand") << QByteArrayLiteral("\xFE\xFE\xE0\xA4\x18\x01\xA4\xFD") << QByteArrayLiteral("\x19\x00") << 1 << false;
}

void CIVProtocol_Private_Test::test_CheckResponse()
{
    // Arrange
    QFETCH(QByteArray, data);
    QFETCH(QByteArray, command);
    QFETCH(int, payloadLength);
    QFETCH(bool, expectedResult);

    // Act
    auto result = _civprotocol->CheckResponse(data, command, payloadLength);

    // Assert
    QCOMPARE(result, expectedResult);
}

void CIVProtocol_Private_Test::test_CreateFrequencyData_data()
{
    QTest::addColumn<int>("frequency");
    QTest::addColumn<QByteArray>("expectedResult");

    QTest::newRow("LowerOutOfRange") << -1 << QByteArrayLiteral("\x00\x00\x00\x00\x00");
    QTest::newRow("UpperOutOfRange") << 1000000000 << QByteArrayLiteral("\x00\x00\x00\x00\x00");
    QTest::newRow("ValidConversion") << 123456789 << QByteArrayLiteral("\x89\x67\x45\x23\x01");
}

void CIVProtocol_Private_Test::test_CreateFrequencyData()
{
    // Arrange
    QFETCH(int, frequency);
    QFETCH(QByteArray, expectedResult);

    // Act
    auto result = _civprotocol->CreateFrequencyData(frequency);

    // Assert
    QCOMPARE(result, expectedResult);
}

void CIVProtocol_Private_Test::test_ParseFrequencyData_data()
{
    QTest::addColumn<QByteArray>("frequencyData");
    QTest::addColumn<int>("expectedResult");

    QTest::newRow("InvalidLength") << QByteArrayLiteral("\x00\x00\x00\x00") << - 1;
    QTest::newRow("ValidConversion") << QByteArrayLiteral("\x89\x67\x45\x23\x01") << 123456789;
}

void CIVProtocol_Private_Test::test_ParseFrequencyData()
{
    // Arrange
    QFETCH(QByteArray, frequencyData);
    QFETCH(int, expectedResult);

    // Act
    auto result = _civprotocol->ParseFrequencyData(frequencyData);

    // Assert
    QCOMPARE(result, expectedResult);
}
