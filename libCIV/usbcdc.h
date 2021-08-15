#pragma once

#include "ICIVcomm.h"
#include <QSerialPort>

class UsbCdc : public ICIVComm
{
    Q_OBJECT

public:
    UsbCdc(const QString portName, const int baudRate, const int txTimeout);
    ~UsbCdc();
    void writeData(const QByteArray& data);

private:
    QSerialPort *_usbCdcPort = nullptr;
    const int _txTimeout;

    void onPortError(QSerialPort::SerialPortError error);

private slots:
    void readData();
};
