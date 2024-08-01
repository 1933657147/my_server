#include "datamanager.h"
#include "widget.h"

QVector<WorkModel> workv;
QVector<UartData> uartv;

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
    db.setDatabaseName("serversql"); //指定数据库
    if(!db.open()){
        qDebug()<<"数据库失败"<<db.lastError().text();
    }
    else {
        qDebug()<<"数据库链接成功";
        logsmanager::writeLogs(QString("数据库链接成功").toUtf8());
    }
    workv.clear();
    uartv.clear();
    QSqlQuery query;
    query.exec("select * from workmode");

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
        workv.push_back(w);
    }

//    qDebug()<<"workv数据如下：";
//    for(auto wv: workv)
//    {
//        qDebug()<<"11111111";
//        qDebug()<<wv.workmode;
//        qDebug()<<"11111111";
//    }

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

//    qDebug()<<"uartv数据如下：";
//    for(auto wv: uartv)
//    {
//        qDebug()<<"11111111";
//        qDebug()<<wv.com_nm;
//        qDebug()<<"11111111";
//    }

}

