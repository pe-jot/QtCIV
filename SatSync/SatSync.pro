QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

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

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    satsync.cpp \
    satsyncgui.cpp

HEADERS += \
    satsync.h \
    satsyncgui.h

FORMS += \
    satsyncgui.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Pack all necessary modules into output directory on release build
CONFIG(release, debug|release): {
    win32: QMAKE_POST_LINK += windeployqt.exe --no-translations --no-system-d3d-compiler --no-webkit2 --no-opengl-sw --no-angle --no-virtualkeyboard $$OUT_PWD/release
}
