#include <QtTest>
#include "tst_civprotocol_gpsinfo_test.h"

CIVProtocol_GPSInfo_Test::CIVProtocol_GPSInfo_Test()
{
}

CIVProtocol_GPSInfo_Test::~CIVProtocol_GPSInfo_Test()
{
}

void CIVProtocol_GPSInfo_Test::test_GPSLatitudeInfo_data()
{
    QTest::addColumn<QByteArray>("inputData");
    QTest::addColumn<bool>("isValid");
    QTest::addColumn<bool>("isNorthern");
    QTest::addColumn<double>("latitude");
    QTest::addColumn<int>("degrees");
    QTest::addColumn<double>("minutes");

    QTest::newRow("Northern") << QByteArrayLiteral("\x48\x12\x34\x56\x01") << true << true << 48.20576 << 48 << 12.3456;
    QTest::newRow("Southern") << QByteArrayLiteral("\x48\x12\x34\x56\x00") << true << false << -48.20576 << -48 << -12.3456;
    QTest::newRow("Invalid") << QByteArrayLiteral("\x48\x12\x34\x00") << false << false << 0.0 << 0 << 0.0;
}

void CIVProtocol_GPSInfo_Test::test_GPSLatitudeInfo()
{
    // Arrange
    QFETCH(QByteArray, inputData);
    QFETCH(bool, isValid);
    QFETCH(bool, isNorthern);
    QFETCH(double, latitude);
    QFETCH(int, degrees);
    QFETCH(double, minutes);
    GPSLatitudeInfo infoObject;

    // Act
    infoObject.fromRawData(inputData);

    // Assert
    QCOMPARE(infoObject.isValid(), isValid);
    QCOMPARE(infoObject.isNorthern(), isNorthern);
    QCOMPARE(infoObject.degrees(), degrees);
    QCOMPARE(infoObject.minutes(), minutes);
    QCOMPARE(infoObject.latitude(), latitude);
}

void CIVProtocol_GPSInfo_Test::test_GPSLongitudeInfo_data()
{
    QTest::addColumn<QByteArray>("inputData");
    QTest::addColumn<bool>("isValid");
    QTest::addColumn<bool>("isEastern");
    QTest::addColumn<double>("longitude");
    QTest::addColumn<int>("degrees");
    QTest::addColumn<double>("minutes");

    QTest::newRow("Eastern") << QByteArrayLiteral("\x01\x48\x12\x34\x56\x01") << true << true << 148.20576 << 148 << 12.3456;
    QTest::newRow("Western") << QByteArrayLiteral("\x01\x48\x12\x34\x56\x00") << true << false << -148.20576 << -148 << -12.3456;
    QTest::newRow("Invalid") << QByteArrayLiteral("\x01\x48\x12\x34\x56\x78\x00") << false << false << 0.0 << 0 << 0.0;
}

void CIVProtocol_GPSInfo_Test::test_GPSLongitudeInfo()
{
    // Arrange
    QFETCH(QByteArray, inputData);
    QFETCH(bool, isValid);
    QFETCH(bool, isEastern);
    QFETCH(double, longitude);
    QFETCH(int, degrees);
    QFETCH(double, minutes);
    GPSLongitudeInfo infoObject;

    // Act
    infoObject.fromRawData(inputData);

    // Assert
    QCOMPARE(infoObject.isValid(), isValid);
    QCOMPARE(infoObject.isEastern(), isEastern);
    QCOMPARE(infoObject.degrees(), degrees);
    QCOMPARE(infoObject.minutes(), minutes);
    QCOMPARE(infoObject.longitude(), longitude);
}

void CIVProtocol_GPSInfo_Test::test_GPSAltitudeInfo_data()
{
    QTest::addColumn<QByteArray>("inputData");
    QTest::addColumn<bool>("isValid");
    QTest::addColumn<double>("altitude");

    QTest::newRow("Positive") << QByteArrayLiteral("\x01\x23\x45\x00") << true << 1234.5;
    QTest::newRow("Negative") << QByteArrayLiteral("\x01\x23\x45\x01") << true << -1234.5;
    QTest::newRow("Invalid") << QByteArrayLiteral("\x12\x34\x00") << false << 0.0;
    QTest::newRow("No3DFix") << QByteArrayLiteral("\xFF\xFF\xFF\xFF") << false << 0.0;
}

void CIVProtocol_GPSInfo_Test::test_GPSAltitudeInfo()
{
    // Arrange
    QFETCH(QByteArray, inputData);
    QFETCH(bool, isValid);
    QFETCH(double, altitude);
    GPSAltitudeInfo infoObject;

    // Act
    infoObject.fromRawData(inputData);

    // Assert
    QCOMPARE(infoObject.isValid(), isValid);
    QCOMPARE(infoObject.altitude(), altitude);
}

void CIVProtocol_GPSInfo_Test::test_GPSCourseInfo_data()
{
    QTest::addColumn<QByteArray>("inputData");
    QTest::addColumn<bool>("isValid");
    QTest::addColumn<int>("course");

    QTest::newRow("Valid") << QByteArrayLiteral("\x01\x23") << true << 123;
    QTest::newRow("Invalid") << QByteArrayLiteral("\x12") << false << 0;
}

void CIVProtocol_GPSInfo_Test::test_GPSCourseInfo()
{
    // Arrange
    QFETCH(QByteArray, inputData);
    QFETCH(bool, isValid);
    QFETCH(int, course);
    GPSCourseInfo infoObject;

    // Act
    infoObject.fromRawData(inputData);

    // Assert
    QCOMPARE(infoObject.isValid(), isValid);
    QCOMPARE(infoObject.course(), course);
}

void CIVProtocol_GPSInfo_Test::test_GPSSpeedInfo_data()
{
    QTest::addColumn<QByteArray>("inputData");
    QTest::addColumn<bool>("isValid");
    QTest::addColumn<double>("speed");

    QTest::newRow("Valid") << QByteArrayLiteral("\x01\x23\x45") << true << 1234.5;
    QTest::newRow("Invalid") << QByteArrayLiteral("\x12\x34") << false << 0.0;
    QTest::newRow("No3DFix") << QByteArrayLiteral("\xFF\xFF\xFF") << false << 0.0;
}

void CIVProtocol_GPSInfo_Test::test_GPSSpeedInfo()
{
    // Arrange
    QFETCH(QByteArray, inputData);
    QFETCH(bool, isValid);
    QFETCH(double, speed);
    GPSSpeedInfo infoObject;

    // Act
    infoObject.fromRawData(inputData);

    // Assert
    QCOMPARE(infoObject.isValid(), isValid);
    QCOMPARE(infoObject.speed(), speed);
}

void CIVProtocol_GPSInfo_Test::test_GPSTimeInfo_data()
{
    QTest::addColumn<QByteArray>("inputData");
    QTest::addColumn<bool>("isValid");
    QTest::addColumn<QDateTime>("timestamp");

    QTest::newRow("Valid") << QByteArrayLiteral("\x20\x21\x01\x28\x11\x22\x33") << true << QDateTime(QDate(2021, 01, 28), QTime(11, 22, 33), Qt::UTC);
    QTest::newRow("Invalid") << QByteArrayLiteral("\x20\x21\x99\x28\x11\x22\x33") << false << QDateTime();
    QTest::newRow("LengthInvalid") << QByteArrayLiteral("\x20\x21\x01\x28\x11\x22") << false << QDateTime();
}

void CIVProtocol_GPSInfo_Test::test_GPSTimeInfo()
{
    // Arrange
    QFETCH(QByteArray, inputData);
    QFETCH(bool, isValid);
    QFETCH(QDateTime, timestamp);
    GPSTimeInfo infoObject;

    // Act
    infoObject.fromRawData(inputData);

    // Assert
    QCOMPARE(infoObject.isValid(), isValid);
    QCOMPARE(infoObject.dateTime(), timestamp);
}

void CIVProtocol_GPSInfo_Test::test_GPSInfo()
{
    // Arrange
    auto inputData = QByteArrayLiteral("\x48\x09\x40\x50\x01\x00\x14\x00\x45\x90\x01\x00\x30\x16\x00\x00\x50\x00\x00\x11\x20\x21\x08\x07\x13\x56\x16");

    // Act
    GPSInfo infoObject(inputData);

    // Assert
    QCOMPARE(infoObject.isValid(), true);
    QCOMPARE(infoObject.speed(), 1.1);
    QCOMPARE(infoObject.latitude(), 48.15675);
    QCOMPARE(infoObject.longitude(), 14.00765);
    QCOMPARE(infoObject.isAltitudeValid(), true);
    QCOMPARE(infoObject.altitude(), 301.6);
    QCOMPARE(infoObject.course(), 50);
    QCOMPARE(infoObject.timestamp(), QDateTime(QDate(2021, 8, 7), QTime(13, 56, 16), Qt::UTC));
}

void CIVProtocol_GPSInfo_Test::test_GPSInfo_InvalidLength()
{
    // Arrange
    auto inputData = QByteArrayLiteral("\xFE\xFE\xE0\xA4\x23\x00\x48\x09\x40\x50\x01\x00\x14\x00\x45\x90\x01\x00\x30\x16\x00\x00\x50\x00\x00\x11\x20\x21\x08\x07\x13\x56\x16\xFD");
    auto expectedPositionText = QString("n/a");
    auto expectedLocatorText = QString("n/a");

    // Act
    GPSInfo infoObject(inputData);
    auto positionText = infoObject.getPositionText();
    auto locatorText = infoObject.getMaidenheadLocatorText();

    // Assert
    QCOMPARE(infoObject.isValid(), false);
    QCOMPARE(positionText, expectedPositionText);
    QCOMPARE(locatorText, expectedLocatorText);
}

void CIVProtocol_GPSInfo_Test::test_GPSInfo_getPositionText_NE()
{
    // Arrange
    auto inputData = QByteArrayLiteral("\x08\x09\x40\x50\x01\x00\x14\x00\x45\x90\x01\x00\x30\x16\x00\x00\x50\x00\x00\x11\x20\x21\x08\x07\x13\x56\x16");
    auto expectedPositionText = QString("8° 9.405' N / 14° 0.459' E");

    // Act
    GPSInfo infoObject(inputData);
    auto result = infoObject.getPositionText();

    // Assert
    QVERIFY(infoObject.isValid());
    QCOMPARE(result, expectedPositionText);
}

void CIVProtocol_GPSInfo_Test::test_GPSInfo_getPositionText_SW()
{
    // Arrange
    auto inputData = QByteArrayLiteral("\x48\x09\x40\x50\x00\x01\x14\x00\x45\x90\x00\x00\x30\x16\x00\x00\x50\x00\x00\x11\x20\x21\x08\x07\x13\x56\x16");
    auto expectedPositionText = QString("48° 9.405' S / 114° 0.459' W");

    // Act
    GPSInfo infoObject(inputData);
    auto result = infoObject.getPositionText();

    // Assert
    QVERIFY(infoObject.isValid());
    QCOMPARE(result, expectedPositionText);
}

void CIVProtocol_GPSInfo_Test::test_GPSInfo_getMaidenheadLocatorText()
{
    // Arrange
    auto inputData = QByteArrayLiteral("\x48\x09\x37\x20\x01\x00\x14\x02\x35\x20\x01\x00\x30\x16\x00\x00\x50\x00\x00\x11\x20\x21\x08\x07\x13\x56\x16");
    auto expectedLocatorText = QString("JN78ad");

    // Act
    GPSInfo infoObject(inputData);
    auto result = infoObject.getMaidenheadLocatorText();

    // Assert
    QVERIFY(infoObject.isValid());
    QCOMPARE(result, expectedLocatorText);
}
