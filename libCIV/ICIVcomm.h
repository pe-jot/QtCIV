#pragma once
#include <QObject>

class ICIVComm : public QObject
{
    Q_OBJECT

public:
    virtual void writeData(const QByteArray& data) = 0;
    bool isOpen() { return _isOpen; }

protected:
    bool _isOpen = false;

signals:
    void dataReceived(const QByteArray data);
};
