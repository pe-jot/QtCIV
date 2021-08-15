#pragma once
#include <QObject>

class IICOMcomm : public QObject
{
    Q_OBJECT

public:
    virtual void init() = 0;
    virtual void writeData(const QByteArray& data) = 0;
    bool isOpen() { return _isOpen; }
    QString getName() { return _name; }

protected:
    bool _isOpen = false;
    QString _name;

signals:
    void initComplete(const bool success);
    void dataReceived(const QList<QByteArray> data);
};
