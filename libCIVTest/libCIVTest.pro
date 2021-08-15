QT += testlib serialport
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_civprotocol_smeter_test.cpp \
    main.cpp \
    tst_civprotocol_cmd_test.cpp \
    tst_civprotocol_create_test.cpp \
    tst_civprotocol_gpsinfo_test.cpp \
    tst_civprotocol_parse_test.cpp \
    tst_civprotocol_private_test.cpp \
    tst_ic705serial_test.cpp

release: LIBS += -L../libCIV/release -llibCIV
debug: LIBS += -L../libCIV/debug -llibCIV
INCLUDEPATH += ../libCIV
PRE_TARGETDEPS += ../libCIV

HEADERS += \
    tst_civprotocol_cmd_test.h \
    tst_civprotocol_create_test.h \
    tst_civprotocol_gpsinfo_test.h \
    tst_civprotocol_parse_test.h \
    tst_civprotocol_private_test.h \
    tst_civprotocol_smeter_test.h \
    tst_ic705serial_test.h
