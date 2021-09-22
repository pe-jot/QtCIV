QT -= gui
QT += network websockets serialport

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Only Windows has these additional debug/release subfolders
win32: {
    release: LIBS += -L../libCIV/release -llibCIV
    debug: LIBS += -L../libCIV/debug -llibCIV
}
else: {
    LIBS += -L../libCIV -llibCIV
}
INCLUDEPATH += ../libCIV
PRE_TARGETDEPS += ../libCIV

SOURCES += \
        civbridge.cpp \
        consolesignalhandler.cpp \
        main.cpp \
        raspigpio.cpp \
        talkkonnectclient.cpp \
        websocketserver.cpp

HEADERS += \
    civbridge.h \
    consolesignalhandler.h \
    raspigpio.h \
    talkkonnectclient.h \
    websocketserver.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
