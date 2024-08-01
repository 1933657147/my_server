#include "datamanager.h"
#include "widget.h"

QVector<WorkModel> workv;//结构体数组，用于存储从数据库表中查询到的所有记录。每个 WorkModel 对象包含了表中的一行数据。
QVector<UartData> uartv;//用于存储从数据库表中查询到的所有记录。每个 UartData 对象包含了表中的一行数据。

dataManager::dataManager(QWidget *parent) : QWidget(parent)
{
}

void dataManager::connect_db()
{
    //链接mysql服务器
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("123456");
    db.setDatabaseName("serversql"); //指定数据库名称

    //连接数据库，如果成功将信息输入到日志文件，如果失败直接控制台打印错误信息
    if(!db.open()){
        qDebug()<<"数据库失败"<<db.lastError().text();
    }
    else {
        qDebug()<<"数据库链接成功";
        logsmanager::writeLogs(QString("数据库链接成功").toUtf8());
    }
    workv.clear();//存放数据之前先清空容器，以便于重新填充
    uartv.clear();//存放数据之前先清空容器，以便于重新填充

    //创建Sql对象，并且执行SQL查询
    QSqlQuery query;
    query.exec("select * from workmode");


    //遍历查询结果，将每一行数据读出并且存储到结构体中
    while (query.next()) {
        // 实例化结构体
        WorkModel w;

        // 根据数据库对结构体进行赋值
        w.id = query.value("id").toInt();
        w.workmode = query.value("workmode").toString();
        w.tcp_datamode = query.value("tcp_datamode").toString();
        w.port_no = query.value("port_no").toInt();
        w.cr_explained = query.value("cr_explained").toString();
        w.lf_explained = query.value("lf_explained").toString();
        w.links_num = query.value("links_num").toInt();
        w.protocol = query.value("protocol").toInt();
        w.peerhost = query.value("peerhost").toString();
        w.peerport = query.value("peerport").toInt();
        w.connect = query.value("connect").toString();
        w.disconnect = query.value("disconnect").toString();
        w.freetime = query.value("freetime").toInt();

        //将结构体对象添加到结构体容器中
        workv.push_back(w);
    }


    query.exec("select * from serial_port");

    while (query.next()) {
        // 实例化结构体
        UartData u;

        // 根据数据库对结构体进行赋值
        u.port_no=query.value("port_no").toInt();
        u.id=query.value("id").toInt();
        u.com_no = query.value("com_no").toInt();
        u.baudrate = query.value("baudrate").toInt();
        u.databit = query.value("databit").toInt();
        u.stopbit = query.value("stopbit").toInt();
        u.checkbit = query.value("checkbit").toString();
        u.flowcontrol = query.value("flowcontrol").toString();
        u.rx_time = query.value("rx_time").toInt();
        u.tx_time = query.value("tx_time").toInt();
        u.com_nm = query.value("com_nm").toString();
        uartv.push_back(u);
    }


}

