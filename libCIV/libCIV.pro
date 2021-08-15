QT -= gui
QT += serialport

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    civprotocol.cpp \
    civprotocol_commands.cpp \
    civprotocol_parser.cpp \
    gpsinfo.cpp \
    ic705serial.cpp \
    usbcdc.cpp

HEADERS += \
    ICIVcomm.h \
    IICOMcomm.h \
    civprotocol.h \
    gpsinfo.h \
    ic705serial.h \
    usbcdc.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
