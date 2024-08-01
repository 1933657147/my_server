#include "widget.h"
#include "loginmanager.h"
#include "datamanager.h"
#include <QApplication>
#include"myudp.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    dataManager data;
    data.connect_db();


//    //调试串口
    mytcpclient mytcp_c;
//    mytcp_c.startUartConfig();
//    mytcp_c.showData();

//    //调试udp
//    myUdp my_udp;
//    my_udp.startUartConfig();


    Widget w;


//    loginManager login;
//    login.show();
    w.show();

//    if(login.exec() == QDialog::Accepted) {
//        w.show();
//        return a.exec();
//    }


    return a.exec();
}
