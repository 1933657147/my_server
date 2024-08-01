#include "tcpclient.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QSerialPort>
#include <QTimer>
#include "datamanager.h"

TcpClient::TcpClient(QObject *parent) : QObject(parent)
{

}

void TcpClient::ClientMode(QSerialPort *serialPort,QString host,int port)
{
    static int count = 0;
    count=0;

    QTcpSocket *clientSocket = new QTcpSocket(this);
    // 设置定时器连接服务器
    QTimer *connectTime = new QTimer(this);
    connectTime->start(5000);

    // 连接串口的readyRead信号来读取串口数据
    connect(serialPort, &QSerialPort::readyRead, [clientSocket,serialPort]() {
        QByteArray data = serialPort->readAll();
        qDebug()<<"TCP client 收到的消息："<<data;

        //找到对应的socket并发送消息
        clientSocket->write(data);
    });

    qDebug()<< QString("当前要链 %1 的对端服务器").arg(port);
    connect(connectTime, &QTimer::timeout, [clientSocket,connectTime,host,port]() {
        // 链接到服务端
        count++;
        qDebug() << QString("TCP Client 第%3次 尝试链接服务器...地址：%1  端口%2 ").arg(host).arg(port).arg(count);
        clientSocket->connectToHost(host,port);
        if(count > 5)
        {
            qDebug()<<"链接服务器失败，请查看服务器是否正常";
            connectTime->stop();
            count = 0;
        }
    });

    // 链接成功
    connect(clientSocket, &QTcpSocket::connected, [this,host,connectTime]() {
        qDebug()<<"TCP Client链接到服务器成功"<<host;
        logsmanager::writeLogs(QString("TCP Client链接到服务器%1成功").arg(host).toUtf8());
        //this->freetime->start(this->workModel.freetime);
        connectTime->stop();
        count=0;
    });

    // 监听readyRead信号，一旦有了服务器传输的信息，就去socket中读取
    //客户端接收服务端的数据  服务端数据发送过来  触发readyread信号
    connect(clientSocket, &QTcpSocket::readyRead, [this,clientSocket,port,host,serialPort]() {
        // 每次读取到数据重置定时器
        //this->freetime->start();
        // 接收服务端的信息：就是从socket中读取服务端写入的数据
        QByteArray data = clientSocket->readAll();

        qDebug()<<port<<"收到来自服务器发来的数据："<<data;
        logsmanager::writeLogs(QString("TCP Client端口收到来自服务器发来的数据：%1 收到消息").arg(port).toUtf8() + data);

        if(serialPort->isOpen()) {
            serialPort->write(data); //发送数据到串口
        }
    });

    // 断开链接
    connect(clientSocket, &QTcpSocket::disconnected, [this,host,connectTime]() {
        qDebug()<<host<<"与服务器断开连接";
        logsmanager::writeLogs(QString("TCP Client断开链接%1").arg(host).toUtf8());
        connectTime->start(1000);
        if(count>5)
        {
            connectTime->stop();
            count=0;
        }
        //this->freetime->stop();

    });

}
