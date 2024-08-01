#include "tcpserver.h"
#include "logsmanager.h"

TcpServer::TcpServer(QObject *parent) : QObject(parent)
{

}

void TcpServer::ServerMode(QSerialPort *serialPort,quint16 localport,int linksNum)
{
    QTcpServer *server = new QTcpServer(this);
    // 监听端口
    server->listen(QHostAddress::Any,localport);

    // 接收到客户端发送来的链接请求，建立链接
    connect(server,&QTcpServer::newConnection,[this,server,serialPort,linksNum](){
        //通过socket获取客户端信息ip和端口
        QTcpSocket *serverSocket = server->nextPendingConnection();
        QString ip =  serverSocket->peerAddress().toString();
        quint16 port =  serverSocket->peerPort();
        QString key = QString("%1-%2").arg(ip).arg(port);
        qDebug()<<QString("%1:%2-->链接到服务器").arg(ip).arg(port);
        logsmanager::writeLogs(QString("%1:%2-->链接到服务器").arg(ip).arg(port).toUtf8());

        //把新来的客户端socket写入容器
        auto it = this->socketMap.find(key);
        if(this->socketMap.size() == linksNum ) {
            qDebug()<<QString("已达到最大链接数%1").arg(linksNum);
            serverSocket->disconnectFromHost();
            serverSocket->close();     //达到最大连接数
            serverSocket->destroyed();
            return;
        }
        else
        {
            if( it == this->socketMap.end()) {
                this->socketMap.insert(key, serverSocket);
            }
        }
        auto it_send = this->socketMap.find(key);


        // 服务器获取到客户端发来的信息时，将数据写入串口
        connect(serverSocket,&QTcpSocket::readyRead,[serverSocket,serialPort,it_send](){
            // 接收socket中读取到的数据
            QByteArray msg = serverSocket->readAll();
            qDebug()<<QString("接收到%1发送的消息: %2").arg(it_send.key()).arg(QString(msg));
            logsmanager::writeLogs(QString("接收到%1发送的消息: %2").arg(it_send.key()).arg( QString(msg)).toUtf8());
            // 将读取的数据写入串口
            serialPort->write(msg);
        });

        // 串口接收到数据后写入serverSocket
        connect(serialPort,&QSerialPort::readyRead,[serialPort,serverSocket,it_send](){
            // 读取串口的发送的数据
            QByteArray data = serialPort->readAll();
            qDebug()<<"串口收到的消息："<<data<<QString("要返回给%1").arg(it_send.key());
            // 将串口数据写入serverSocket中
            it_send.value()->write(data);
        });

        //监听 disconnected
        connect(serverSocket, &QTcpSocket::disconnected, [this, it_send]() {
            qDebug()<<QString("%1断开了链接").arg(it_send.key());
            logsmanager::writeLogs(QString("%1断开了链接").arg(it_send.key()).toUtf8());
            this->socketMap.remove(it_send.key());
        });



    });
}
