#pragma once
#include <QObject>
#include "gpsinfo.h"

class CIVProtocol_GPSInfo_Test : public QObject
{
    Q_OBJECT

public:
    CIVProtocol_GPSInfo_Test();
    ~CIVProtocol_GPSInfo_Test();

private slots:
    void test_GPSLatitudeInfo_data();
    void test_GPSLatitudeInfo();

    void test_GPSLongitudeInfo_data();
    void test_GPSLongitudeInfo();

    void test_GPSAltitudeInfo_data();
    void test_GPSAltitudeInfo();

    void test_GPSSpeedInfo_data();
    void test_GPSSpeedInfo();

    void test_GPSCourseInfo_data();
    void test_GPSCourseInfo();

    void test_GPSTimeInfo_data();
    void test_GPSTimeInfo();

    void test_GPSInfo();
    void test_GPSInfo_InvalidLength();
    void test_GPSInfo_getPositionText_NE();
    void test_GPSInfo_getPositionText_SW();
    void test_GPSInfo_getMaidenheadLocatorText();
};
