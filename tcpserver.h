#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QSerialPort>
#include <QTcpSocket>
#include <QTcpServer>


class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);

    void ServerMode(QSerialPort *serialPort,quint16 localport,int linksNum);
private:
    QMap<QString, QTcpSocket *>  socketMap; //连接对象

signals:

public slots:
};

#endif // TCPSERVER_H
