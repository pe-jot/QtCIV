#include "ic705serial.h"
#include "usbcdc.h"
#include <QSerialPortInfo>
#include <QDebug>

IC705Serial::IC705Serial()
{    
    _civ = new CIVProtocol();
}


IC705Serial::~IC705Serial()
{
    _isOpen = false;

    if (_comm != nullptr)
    {
        disconnect(_comm, &ICIVComm::dataReceived, this, &IC705Serial::dataReceivedSlot);
        delete _comm;
        _comm = nullptr;
    }

    if (_civ != nullptr)
    {
        delete _civ;
        _civ = nullptr;
    }
}


void IC705Serial::init()
{
    QString portName = FindIcomPortName();
    if (portName == nullptr)
    {
        emit initComplete(false);
        return;
    }

    _comm = new UsbCdc(portName, BaudRate, TxTimeout);

    if (!_comm->isOpen())
    {
        emit initComplete(false);
        return;
    }

    connect(_comm, &ICIVComm::dataReceived, this, &IC705Serial::dataReceivedSlot);
    _name = portName;
    _isOpen = true;
    emit initComplete(true);
}


void IC705Serial::writeData(const QByteArray& data)
{
    if (_comm && _comm->isOpen())
    {
        _comm->writeData(data);
    }
}


void IC705Serial::dataReceivedSlot(const QByteArray data)
{
    auto receivedMessages = SplitReceivedMessages(data);
    emit dataReceived(receivedMessages);
}


QList<QByteArray> IC705Serial::SplitReceivedMessages(const QByteArray& data)
{
    const char start = char(0xFE);
    const char end = char(0xFD);

    QList<QByteArray> results;

    int startPos = -1;
    for (int i = 0; i < data.length(); i++)
    {
        if (data.at(i) == start && data.at(i + 1) == start)
        {
            startPos = i;
        }
        else if (data.at(i) == end)
        {
            if (startPos >= 0)
            {
                results.append(data.mid(startPos, i - startPos + 1));
            }
            startPos = -1;
        }
    }

    return results;
}


bool IC705Serial::TryGetTransceiverId(QString portName)
{
    auto testPort = new QSerialPort(portName);
    testPort->setBaudRate(BaudRate);
    testPort->clearError();

    if (!testPort->open(QIODevice::ReadWrite))
    {
        qWarning() << testPort->error() << testPort->errorString();
        return false;
    }
    testPort->clear();

    auto request = _civ->CmdReadTransceiverId();
    qDebug() << "Request:" << request.length() << "Bytes" << request;
    testPort->write(request);
    testPort->waitForBytesWritten(TxTimeout);

    auto expectedResponse = _civ->ResponseReadTransceiverId();
    testPort->waitForReadyRead(TxTimeout);
    auto response = testPort->readAll();
    qDebug() << "Response:" << response.length() << "Bytes" << response;

    testPort->close();

    return (response == expectedResponse);
}


QString IC705Serial::FindIcomPortName()
{
    auto availablePorts = QSerialPortInfo::availablePorts();
    for (auto port = availablePorts.begin(); port < availablePorts.end(); ++port)
    {
        auto portName = port->portName();
        auto vid = port->hasVendorIdentifier() ? port->vendorIdentifier() : 0;
        auto pid = port->hasProductIdentifier() ? port->productIdentifier() : 0;

        qDebug().noquote() << "Port:" << portName << "VID:" << QString::number(vid, 16) << "PID:" << QString::number(pid, 16);

        if (vid == IC705Serial::ICOM_USB_VID && pid == IC705Serial::IC705_USB_PID)
        {
            bool found = TryGetTransceiverId(portName);
            if (found)
            {
                qDebug() << "Found transceiver on" << portName;
                return portName;
            }
        }
    }

    return nullptr;
}
