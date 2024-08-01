#ifndef USERSMANAGER_H
#define USERSMANAGER_H

#include <QWidget>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include "logsmanager.h"
#include "loginmanager.h"
#include<QMenu>

#include <QMouseEvent>

namespace Ui {
class usersmanager;
}

class usersmanager : public QWidget
{
    Q_OBJECT

public:
    explicit usersmanager(QWidget *parent = nullptr);
    ~usersmanager();
    void showTables();
    void clear_info();
    QString confirm_flag;
    QSqlQuery query;
    QString sql;
    int column=5;
    int curr_row=-1;

private slots:
    void on_addusers_clicked();

    void on_confirm_clicked();

    void on_cancel_clicked();

    void on_delusers_clicked();

    void on_modefyusers_clicked();

    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

    void on_toolButton_clicked();

private:
    Ui::usersmanager *ui;
    int clickx = 0;
    int clicky = 0;
    QMenu *cMenu;
};

#endif // USERSMANAGER_H
