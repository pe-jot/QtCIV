#include "usbcdc.h"
#include <QDebug>

UsbCdc::UsbCdc(const QString portName, const int baudRate, const int txTimeout)
    : _txTimeout(txTimeout)
{
    _usbCdcPort = new QSerialPort(portName);
    _usbCdcPort->setBaudRate(baudRate);
    _usbCdcPort->clearError();

#ifdef QT_DEBUG
    QSerialPort::connect(_usbCdcPort, &QSerialPort::errorOccurred, this, &UsbCdc::onPortError);
#endif

    if (!_usbCdcPort->open(QIODevice::ReadWrite))
    {
        return;
    }

    _isOpen = _usbCdcPort->isOpen();
    _usbCdcPort->clear();
    QSerialPort::connect(_usbCdcPort, &QSerialPort::readyRead, this, &UsbCdc::readData);
}


UsbCdc::~UsbCdc()
{
    if (_usbCdcPort != nullptr)
    {
        QSerialPort::disconnect(_usbCdcPort, &QSerialPort::readyRead, this, &UsbCdc::readData);
        if (_usbCdcPort->isOpen())
        {
            _usbCdcPort->clear();
            _usbCdcPort->close();
            _usbCdcPort->clearError();
            _isOpen = _usbCdcPort->isOpen();
        }
        delete _usbCdcPort;
        _usbCdcPort = nullptr;
    }
}


void UsbCdc::onPortError(QSerialPort::SerialPortError error)
{
    if (error != QSerialPort::NoError)
    {
        qDebug() << error;
    }
}


void UsbCdc::readData()
{
    if (_usbCdcPort == nullptr || !_usbCdcPort->isOpen())
    {
        return;
    }

    auto readData = _usbCdcPort->readAll();
    emit dataReceived(readData);
}


void UsbCdc::writeData(const QByteArray& data)
{
    if (_usbCdcPort == nullptr || !_usbCdcPort->isOpen())
    {
        return;
    }
    _usbCdcPort->write(data);
    _usbCdcPort->waitForBytesWritten(_txTimeout);
}
