#include "mytcpclient.h"
#include "datamanager.h"
#include "tcpclient.h"
#include "tcpserver.h"
#include "myudp.h"


QMap<int, QSerialPort *> serialPortMap;

mytcpclient::mytcpclient(QObject *parent) : QObject(parent)
{
    for(int i=0;i<uartv.size();i++ ) {
        QSerialPort *serialPort = new QSerialPort();
        serialPortMap.insert(i,serialPort);
    }
    this->startUartConfig();

}



void mytcpclient::startUartConfig()
{
    for(int i=0;i<serialPortMap.size();i++ )
    {
        //配置串口的信息
        //链接名
        serialPortMap[i]->setPortName(uartv[i].com_nm);
        //设置波特率
        if(uartv[i].baudrate== 9600){
            serialPortMap[i]->setBaudRate(QSerialPort::Baud9600);
        }
        else if (uartv[i].baudrate== 115200) {
            serialPortMap[i]->setBaudRate(QSerialPort::Baud115200);
        }
        //设置数据位
        if(uartv[i].databit ==5) {
            serialPortMap[i]->setDataBits( QSerialPort::Data5);
        }
        else if (uartv[i].databit ==6) {
            serialPortMap[i]->setDataBits( QSerialPort::Data6);
        }
        else if (uartv[i].databit ==7) {
            serialPortMap[i]->setDataBits( QSerialPort::Data7);
        }
        else if (uartv[i].databit ==8) {
            serialPortMap[i]->setDataBits( QSerialPort::Data8);
        }
        //设置停止位
        if( uartv[i].stopbit == 1) {
            serialPortMap[i]->setStopBits( QSerialPort::OneStop);
        }
        else if (uartv[i].stopbit == 2) {
            serialPortMap[i]->setStopBits( QSerialPort::TwoStop);
        }
        else if (uartv[i].stopbit == 3) {
            serialPortMap[i]->setStopBits( QSerialPort::OneAndHalfStop);
        }
        //设置校验位
        if( uartv[i].checkbit == "None") {
            serialPortMap[i]->setParity(QSerialPort::NoParity);
        }
        else if (uartv[i].checkbit == "奇校验") {
            serialPortMap[i]->setParity(QSerialPort::EvenParity);
        }
        else if (uartv[i].checkbit == "偶校验") {
            serialPortMap[i]->setParity(QSerialPort::OddParity);
        }


        // 打开串口
        if (serialPortMap[i]->open(QIODevice::ReadWrite)) {
            qDebug()<<QString("打开串口(%1)").arg(serialPortMap[i]->portName());
            logsmanager::writeLogs(QString("打开串口(%1)").arg(serialPortMap[i]->portName()).toUtf8());


            for (auto wkitem:workv) {
                if(wkitem.port_no==uartv[i].port_no)
                {
                    if(wkitem.workmode=="TCP Client")
                    {
                        TcpClient *client = new TcpClient();
                        client->ClientMode(serialPortMap[i],wkitem.peerhost,wkitem.peerport);
                    }
                    else if (wkitem.workmode == "TCP Server") {
                        TcpServer *server = new TcpServer();
                        server->ServerMode(serialPortMap[i],(quint16)wkitem.port_no,wkitem.links_num);
                    }
                    else if (wkitem.workmode == "UDP") {
                        myUdp *udp = new myUdp();
                        udp->UdpMode(serialPortMap[i],wkitem.peerhost,wkitem.peerport,wkitem.port_no);
                    }

                }
            }
        }
        else {
            qDebug()<<QString("打开串口失败");
            logsmanager::writeLogs(QString("打开串口失败").toUtf8());
        }

}
}

////初始化的数据
//void mytcpclient::showData()
//{
//}


































////串口初始化
//void mytcpclient::startUartConfig() {

//    QSerialPort *serialPort = new QSerialPort(this);
//    this->count=0;
//    for(auto portInfo : uartv ) {

////        serialPort->close();
//        //配置串口的信息
//        //链接名
//        serialPort->setPortName(portInfo.com_nm);
//        //设置波特率
//        if(portInfo.baudrate== 9600){
//            serialPort->setBaudRate(QSerialPort::Baud9600);
//        }
//        else if (portInfo.baudrate== 115200) {
//            serialPort->setBaudRate(QSerialPort::Baud115200);
//        }
//        //设置数据位
//        if(portInfo.databit ==5) {
//            serialPort->setDataBits( QSerialPort::Data5);
//        }
//        else if (portInfo.databit ==6) {
//            serialPort->setDataBits( QSerialPort::Data6);
//        }
//        else if (portInfo.databit ==7) {
//            serialPort->setDataBits( QSerialPort::Data7);
//        }
//        else if (portInfo.databit ==8) {
//            serialPort->setDataBits( QSerialPort::Data8);
//        }
//        //设置停止位
//        if( portInfo.stopbit == 1) {
//            serialPort->setStopBits( QSerialPort::OneStop);
//        }
//        else if (portInfo.stopbit == 2) {
//            serialPort->setStopBits( QSerialPort::TwoStop);
//        }
//        else if (portInfo.stopbit == 3) {
//            serialPort->setStopBits( QSerialPort::OneAndHalfStop);
//        }
//        //设置校验位
//        if( portInfo.checkbit == "None") {
//            serialPort->setParity(QSerialPort::NoParity);
//        }
//        else if (portInfo.checkbit == "奇校验") {
//            serialPort->setParity(QSerialPort::EvenParity);
//        }
//        else if (portInfo.checkbit == "偶校验") {
//            serialPort->setParity(QSerialPort::OddParity);
//        }

//        //qDebug()<<"开始打开串口，情况如下：";
//        // 打开串口
//        if (serialPort->open(QIODevice::ReadWrite)) {
//            qDebug()<<QString("打开串口(%1)").arg(serialPort->portName());
//            logsmanager::writeLogs(QString("打开串口(%1)").arg(serialPort->portName()).toUtf8());


//            serialPortMap.insert(portInfo.port_no,serialPort);

//            for (auto wkitem:workv) {
//                if(wkitem.port_no==portInfo.port_no)
//                {
//                    if(wkitem.workmode=="TCP Client")
//                    {
//                        TcpClient *client = new TcpClient(this);
//                        client->ClientMode(serialPort,wkitem.peerhost,wkitem.peerport);
//                    }
//                    else if (wkitem.workmode == "TCP Server") {
//                        TcpServer *server = new TcpServer(this);
//                        server->ServerMode(serialPort,(quint16)wkitem.port_no,wkitem.links_num);
//                    }
//                    else if (wkitem.workmode == "UDP") {
//                        myUdp *udp = new myUdp(this);
//                        udp->UdpMode(serialPort,wkitem.peerhost,wkitem.peerport,wkitem.port_no);
//                    }

////                    if(wkitem.workmode=="TCP Client")
////                    {
////                        TcpClient client;
////                        client.ClientMode(serialPort,wkitem.peerhost,wkitem.peerport);
////                    }
////                    else if (wkitem.workmode == "TCP Server") {
////                        TcpServer server;
////                        server.ServerMode(serialPort,(quint16)wkitem.port_no,wkitem.links_num);
////                    }
////                    else if (wkitem.workmode == "UDP") {
////                        myUdp udp;
////                        udp.UdpMode(serialPort,wkitem.peerhost,wkitem.peerport,wkitem.port_no);
////                    }


//                }
//            }
//        }
//        else {
//            qDebug()<<QString("打开串口失败");
//            logsmanager::writeLogs(QString("打开串口失败").toUtf8());
////            serialPort->close();
////            serialPort->destroyed();
////            delete  serialPort;


//        }

//    }
//}
