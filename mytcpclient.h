#ifndef MYTCPCLIENT_H
#define MYTCPCLIENT_H

#include <QObject>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTcpSocket>
#include <QTimer>


extern QMap<int,QSerialPort *> serialPortMap;

class mytcpclient : public QObject
{
    Q_OBJECT
public:
    explicit mytcpclient(QObject *parent = nullptr);
//    QTcpSocket *clientSocket;
//    QSerialPort  *serialPort; // 串口
    static void startUartConfig(); // 开启串口
    //void startUartConfig(int i); // 开启串口
//    WorkModel workModel; // 工作模式结构体
//    UartData  uartData; // 端口结构体
    int rxLength = 0;
    int txLength = 0;
    void close_socket();
    void startupWorkmode(); // 开启工作模式

    QTimer *freetime; // 定时器
//    QTimer *connectTime; // 连接服务器定时器
    int count=0;

//    dataManager *dataInit ;
    void showData();
//    void init_socket(WorkModel work, UartData uartData, int id); // 实例化QTcpServer及对应串口
//    int id;


    void startWork();

private:

signals:



public slots:
};

#endif // MYTCPCLIENT_H
