#pragma once

#include "IICOMcomm.h"
#include "civprotocol.h"
#include "ICIVcomm.h"
#include <QObject>
#include <QSerialPort>

class IC705Serial : public IICOMcomm
{
    Q_OBJECT

    friend class IC705Serial_Test;

public:
    IC705Serial();
    ~IC705Serial();
    void init(void);
    void writeData(const QByteArray& data);

private:
    static const int BaudRate = QSerialPort::Baud115200;
    static const quint16 ICOM_USB_VID  = 0x0C26;
    static const quint16 IC705_USB_PID = 0x0036;
    static const int TxTimeout = 1000;

    CIVProtocol *_civ = nullptr;
    ICIVComm *_comm = nullptr;

    QString FindIcomPortName();
    bool TryGetTransceiverId(QString portName);

    void dataReceivedSlot(const QByteArray data);
    QList<QByteArray> SplitReceivedMessages(const QByteArray& data);
};
