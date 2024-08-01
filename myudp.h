#ifndef MYUDP_H
#define MYUDP_H

#include <QObject>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QUdpSocket>
#include <QTimer>

class myUdp : public QObject
{
    Q_OBJECT
public:
    explicit myUdp(QObject *parent = nullptr);

    void UdpMode(QSerialPort *serialPort,QString peerhost,quint16 peerport,int localport);
signals:

public slots:
};

#endif // MYUDP_H
