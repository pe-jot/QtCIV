#pragma once

#include <QObject>
#include <QSerialPort>

class UsbCdc : public QObject
{
    Q_OBJECT

public:
    UsbCdc(const QString portName, const int baudRate, const int txTimeout);
    ~UsbCdc();
    void writeData(const QByteArray& data);
    void writeDTR(const bool& value);
    void writeRTS(const bool& value);
    bool isOpen() { return _isOpen; }

signals:
    void dataReceived(const QByteArray data);

private:
    QSerialPort *_usbCdcPort = nullptr;
    const int _txTimeout;
    bool _isOpen = false;

    void onPortError(QSerialPort::SerialPortError error);

private slots:
    void readData();
};
