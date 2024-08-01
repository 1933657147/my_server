#ifndef UARTMANAGER_H
#define UARTMANAGER_H

#include <QWidget>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include "logsmanager.h"
#include "loginmanager.h"
#include<QMenu>

#include <QMouseEvent>
namespace Ui {
class uartmanager;
}

class uartmanager : public QWidget
{
    Q_OBJECT

public:
    explicit uartmanager(QWidget *parent = nullptr);
    ~uartmanager();
    void showTables();
    void clear_info();
    QSqlQuery query;
    int column=11;
    int curr_row=-1;
    QString confirm_flag;
    QString sql;

private slots:
    void on_deleteBtn_clicked();

    void on_editBtn_clicked();

    void on_saveBtn_clicked();

    void on_addBtn_clicked();

    void on_cancelBtn_clicked();

    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

    void on_toolButton_clicked();

private:
    Ui::uartmanager *ui;
    int clickx = 0;
    int clicky = 0;
    QMenu *cMenu;
};

#endif // UARTMANAGER_H
