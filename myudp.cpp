#include "myudp.h"
#include"datamanager.h"
myUdp::myUdp(QObject *parent) : QObject(parent)
{

}

void myUdp::UdpMode(QSerialPort *serialPort,QString peerhost,quint16 peerport,int localport)
{
    QUdpSocket *udpSocket = new QUdpSocket(this);
    udpSocket->bind(localport);

    qDebug()<<QString("成功绑定端口%1").arg(localport);
    // 监听readyRead信号，一旦有了传输的信息，就去socket中读取
    //接收发来的数据  服务端数据发送过来  触发readyread信号
    connect(udpSocket, &QUdpSocket::readyRead, [udpSocket,serialPort]() {
        // 接收信息：就是从socket中读取写入的数据
        char buf[1024]={0};
//        QByteArray buf;
        QHostAddress ip;
        quint16 port;
        udpSocket->readDatagram(buf,1034,&ip,&port);

        qDebug()<<port<<"收到来自udp发来的数据："<<buf;
        logsmanager::writeLogs(QString("udp端口收到来自服务器发来的数据：%1 收到消息").arg(port).toUtf8() + (QByteArray)buf);

        if(serialPort->isOpen()) {
            serialPort->write(buf); //发送数据到串口
        }
    });

    // 连接串口的readyRead信号来读取串口数据
    connect(serialPort, &QSerialPort::readyRead, [udpSocket,serialPort,peerhost,peerport]() {
        QByteArray data = serialPort->readAll();
        qDebug()<<"udpSocket 收到的消息："<<data;
       // this->txLength += data.size();
        //找到对应的socket并发送消息
        qDebug()<<"udpSocket 转发的消息："<<data;
        udpSocket->writeDatagram(data,QHostAddress(peerhost),peerport);
    });
}

