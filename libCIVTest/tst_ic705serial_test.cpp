#include <QtTest>
#include "tst_ic705serial_test.h"

IC705Serial_Test::IC705Serial_Test()
{
    _ic705serial = new IC705Serial();
}

IC705Serial_Test::~IC705Serial_Test()
{
    delete _ic705serial;
}

void IC705Serial_Test::test_SplitReceivedMessages_data()
{
    QTest::addColumn<QByteArray>("inputData");
    QTest::addColumn<QList<QByteArray>>("expectedResult");

    QTest::newRow("TypicalSingleMessage")
            << QByteArrayLiteral("\xFE\xFE\xE0\xA4\xFB\xFD")
            << QList<QByteArray>{ QByteArrayLiteral("\xFE\xFE\xE0\xA4\xFB\xFD") };

    QTest::newRow("TypicalMultiMessage")
            << QByteArrayLiteral("\xFE\xFE\xE0\xA4\xFB\xFD\xFE\xFE\xE0\xA4\xFA\xFD")
            << QList<QByteArray>{ QByteArrayLiteral("\xFE\xFE\xE0\xA4\xFB\xFD"), QByteArrayLiteral("\xFE\xFE\xE0\xA4\xFA\xFD") };

    QTest::newRow("EndNotFound_NoResult")
            << QByteArrayLiteral("\xFE\xFE\xE0\xA4\xFB")
            << QList<QByteArray>();

    QTest::newRow("StartNotFound_NoResult")
            << QByteArrayLiteral("\xE0\xA4\xFB\xFD")
            << QList<QByteArray>();

    QTest::newRow("RandomData_NoResult")
            << QByteArrayLiteral("abcdefghijklmnopqrstuvwxyz")
            << QList<QByteArray>();

    QTest::newRow("IncompleteStart_NoResult")
            << QByteArrayLiteral("\xFE\xE0\xA4\xFB\xFD")
            << QList<QByteArray>();

    QTest::newRow("MultiMessageIncompleteStart")
            << QByteArrayLiteral("\xA4\xFB\xFD\xFE\xFE\xE0\xA4\xFA\xFD")
            << QList<QByteArray>{ QByteArrayLiteral("\xFE\xFE\xE0\xA4\xFA\xFD") };

    QTest::newRow("MultiMessageIncompleteEnd")
            << QByteArrayLiteral("\xFE\xFE\xE0\xA4\xFB\xFD\xFE\xFE\xE0")
            << QList<QByteArray>{ QByteArrayLiteral("\xFE\xFE\xE0\xA4\xFB\xFD") };
}

void IC705Serial_Test::test_SplitReceivedMessages()
{
    // Arrange
    QFETCH(QByteArray, inputData);
    QFETCH(QList<QByteArray>, expectedResult);

    // Act
    auto result = _ic705serial->SplitReceivedMessages(inputData);

    // Assert
    QCOMPARE(result.length(), expectedResult.length());
    QCOMPARE(result, expectedResult);
}
