#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QSerialPort>

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = nullptr);

    void ClientMode(QSerialPort *serialPort,QString host,int port);

signals:

public slots:
};

#endif // TCPCLIENT_H
