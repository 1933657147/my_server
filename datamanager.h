#ifndef DATAMANAGER_H
#define DATAMANAGER_H
#include "logsmanager.h"

#include <QObject>
#include <QWidget>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QVector>



struct WorkModel {
    int id;
    int protocol;
    QString workmode;
    QString tcp_datamode;
    int port_no;            // 本地端口
    QString cr_explained;
    QString lf_explained;
    int links_num;
    QString peerhost;
    quint16 peerport;
    QString connect;
    QString disconnect;
    int freetime;
};

struct UartData {
    int port_no;
    int id;
    int com_no;
    int baudrate;
    int databit;
    int stopbit;
    QString checkbit;
    QString flowcontrol;
    int rx_time;
    int tx_time;
    QString com_nm;
};

extern QVector<WorkModel> workv;
extern QVector<UartData> uartv;

class dataManager : public QWidget
{
    Q_OBJECT
public:
    explicit dataManager(QWidget *parent = nullptr);
    void connect_db();
    void init_network();


//    QVector<WorkModel> workv;
//    QVector<UartData> uartv;


private:


signals:

public slots:
};

#endif // DATAMANAGER_H
