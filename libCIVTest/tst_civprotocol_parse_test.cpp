#include <QtTest>
#include "tst_civprotocol_parse_test.h"

CIVProtocol_Parse_Test::CIVProtocol_Parse_Test()
{
    _civprotocol = new CIVProtocol();
}

CIVProtocol_Parse_Test::~CIVProtocol_Parse_Test()
{
    delete _civprotocol;
}

void CIVProtocol_Parse_Test::test_ParseSquelchStatus_data()
{
    QTest::addColumn<QByteArray>("inputData");
    QTest::addColumn<qint32>("expectedResult");

    QTest::newRow("InvalidData") << QByteArrayLiteral("\xFE\xFE\xE0\xA4\x15\x01\x00\x00\xFD") << -1;
    QTest::newRow("ValidData") << QByteArrayLiteral("\xFE\xFE\xE0\xA4\x15\x01\x01\xFD") << 1;
}

void CIVProtocol_Parse_Test::test_ParseSquelchStatus()
{
    // Arrange
    QFETCH(QByteArray, inputData);
    QFETCH(qint32, expectedResult);

    // Act
    auto result = _civprotocol->ParseSquelchStatus(inputData);

    // Assert
    QCOMPARE(result, expectedResult);
}

void CIVProtocol_Parse_Test::test_ParseOvfStatus_data()
{
    QTest::addColumn<QByteArray>("inputData");
    QTest::addColumn<qint32>("expectedResult");

    QTest::newRow("InvalidData") << QByteArrayLiteral("\xFE\xFE\xE0\xA4\x15\x07\x00\x00\xFD") << -1;
    QTest::newRow("ValidData") << QByteArrayLiteral("\xFE\xFE\xE0\xA4\x15\x07\x01\xFD") << 1;
}

void CIVProtocol_Parse_Test::test_ParseOvfStatus()
{
    // Arrange
    QFETCH(QByteArray, inputData);
    QFETCH(qint32, expectedResult);

    // Act
    auto result = _civprotocol->ParseOvfStatus(inputData);

    // Assert
    QCOMPARE(result, expectedResult);
}

void CIVProtocol_Parse_Test::test_ParseRxFrequencyResponse_data()
{
    QTest::addColumn<QByteArray>("inputData");
    QTest::addColumn<qint32>("expectedResult");

    QTest::newRow("InvalidData") << QByteArrayLiteral("\xFE\xFE\xE0\xA4\x33\x89\x67\x45\x23\x01\xFD") << -1;
    QTest::newRow("ValidData") << QByteArrayLiteral("\xFE\xFE\xE0\xA4\x03\x89\x67\x45\x23\x01\xFD") << 123456789;
}

void CIVProtocol_Parse_Test::test_ParseRxFrequencyResponse()
{
    // Arrange
    QFETCH(QByteArray, inputData);
    QFETCH(qint32, expectedResult);

    // Act
    auto result = _civprotocol->ParseRxFrequencyResponse(inputData);

    // Assert
    QCOMPARE(result, expectedResult);
}

void CIVProtocol_Parse_Test::test_ParseTxFrequencyResponse_data()
{
    QTest::addColumn<QByteArray>("inputData");
    QTest::addColumn<qint32>("expectedResult");

    QTest::newRow("InvalidData") << QByteArrayLiteral("\xFE\xFE\xE0\xA4\x1C\x33\x89\x67\x45\x23\x01\xFD") << -1;
    QTest::newRow("ValidData") << QByteArrayLiteral("\xFE\xFE\xE0\xA4\x1C\x03\x89\x67\x45\x23\x01\xFD") << 123456789;
}

void CIVProtocol_Parse_Test::test_ParseTxFrequencyResponse()
{
    // Arrange
    QFETCH(QByteArray, inputData);
    QFETCH(qint32, expectedResult);

    // Act
    auto result = _civprotocol->ParseTxFrequencyResponse(inputData);

    // Assert
    QCOMPARE(result, expectedResult);
}

void CIVProtocol_Parse_Test::test_ParseRxFrequencyChange_data()
{
    QTest::addColumn<QByteArray>("inputData");
    QTest::addColumn<qint32>("expectedResult");

    QTest::newRow("InvalidData") << QByteArrayLiteral("\xFE\xFE\x00\xA4\x11\x89\x67\x45\x23\x01\xFD") << -1;
    QTest::newRow("ValidData") << QByteArrayLiteral("\xFE\xFE\x00\xA4\x00\x89\x67\x45\x23\x01\xFD") << 123456789;
}

void CIVProtocol_Parse_Test::test_ParseRxFrequencyChange()
{
    // Arrange
    QFETCH(QByteArray, inputData);
    QFETCH(qint32, expectedResult);

    // Act
    auto result = _civprotocol->ParseRxFrequencyChange(inputData);

    // Assert
    QCOMPARE(result, expectedResult);
}


void CIVProtocol_Parse_Test::test_ParseSMeter_data()
{
    QTest::addColumn<QByteArray>("inputData");
    QTest::addColumn<qint32>("expectedResult");

    QTest::newRow("0") << QByteArrayLiteral("\xFE\xFE\xE0\xA4\x15\x02\x00\x00\xFD") << 0;
    QTest::newRow("99") << QByteArrayLiteral("\xFE\xFE\xE0\xA4\x15\x02\x00\x99\xFD") << 99;
    QTest::newRow("100") << QByteArrayLiteral("\xFE\xFE\xE0\xA4\x15\x02\x01\x00\xFD") << 100;
    QTest::newRow("255") << QByteArrayLiteral("\xFE\xFE\xE0\xA4\x15\x02\x02\x55\xFD") << 255;
    QTest::newRow("InvalidLength") << QByteArrayLiteral("\xFE\xFE\xE0\xA4\x15\x02\x03\xFD") << -1;
    QTest::newRow("InvalidData") << QByteArrayLiteral("\xFE\xFE\xE0\xA4\x15\x01\x02\x03\xFD") << -1;
}

void CIVProtocol_Parse_Test::test_ParseSMeter()
{
    // Arrange
    QFETCH(QByteArray, inputData);
    QFETCH(qint32, expectedResult);

    // Act
    auto result = _civprotocol->ParseSMeter(inputData);

    // Assert
    QCOMPARE(result, expectedResult);
}


void CIVProtocol_Parse_Test::test_ParseGpsStatusResponseData_OK()
{
    // Arrange
    // Lat: "48 09 40 50 01" Lon: "00 14 00 45 90 01" Alt: "00 30 16 00" Cse: "00 50" Spd: "00 00 11" Tme: "20 21 08 07 13 56 16"
    auto inputData = QByteArrayLiteral("\xFE\xFE\xE0\xA4\x23\x00\x48\x09\x40\x50\x01\x00\x14\x00\x45\x90\x01\x00\x30\x16\x00\x00\x50\x00\x00\x11\x20\x21\x08\x07\x13\x56\x16\xFD");

    // Act
    auto result = _civprotocol->ParseGpsStatusResponseData(inputData);

    // Assert
    QVERIFY(result != nullptr);
    QCOMPARE(result->isValid(), true);
    QCOMPARE(result->speed(), 1.1);
    QCOMPARE(result->latitude(), 48.15675);
    QCOMPARE(result->longitude(), 14.00765);
    QCOMPARE(result->isAltitudeValid(), true);
    QCOMPARE(result->altitude(), 301.6);
    QCOMPARE(result->course(), 50);
    QCOMPARE(result->timestamp(), QDateTime(QDate(2021, 8, 7), QTime(13, 56, 16), Qt::UTC));

    // Teardown
    delete result;
}

void CIVProtocol_Parse_Test::test_ParseGpsStatusResponseData_NoGpsFix()
{
    // Arrange
    auto inputData = QByteArrayLiteral("\xFE\xFE\xE0\xA4\x23\x00\xFF\xFD");

    // Act
    auto result = _civprotocol->ParseGpsStatusResponseData(inputData);

    // Assert
    QVERIFY(result != nullptr);
    QCOMPARE(result->isValid(), false);

    // Teardown
    delete result;
}

void CIVProtocol_Parse_Test::test_ParseGpsStatusResponseData_InvalidLength()
{
    // Arrange
    auto inputData = QByteArrayLiteral("\xFE\xFE\xE0\xA4\x23\x00\x48\x09\x40\x50\x01\x00\x14\x00\x45\x90\x01\x00\x30\x16\x00\x00\x50\x00\x00\x11xFD");

    // Act
    auto result = _civprotocol->ParseGpsStatusResponseData(inputData);

    // Assert
    QVERIFY(result == nullptr);
}
