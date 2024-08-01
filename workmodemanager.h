#ifndef WORKMODEMANAGER_H
#define WORKMODEMANAGER_H

#include <QWidget>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include "logsmanager.h"
#include "loginmanager.h"
#include<QMenu>

#include <QMouseEvent>
namespace Ui {
class workmodemanager;
}

class workmodemanager : public QWidget
{
    Q_OBJECT

public:
    explicit workmodemanager(QWidget *parent = nullptr);
    ~workmodemanager();
    void showTables();
    void clear_info();
    void unusedPort_no();
    QSqlQuery query;
    int column=14;
    int curr_row=-1;
    QString confirm_flag;
    QString sql;

private slots:
    void on_workmodeBox_currentTextChanged(const QString &arg1);

    void on_addBtn_clicked();

    void on_modifyBtn_clicked();

    void on_certainBtn_clicked();

    void on_delBtn_clicked();

    void on_cancelBtn_clicked();

    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

    void on_toolButton_clicked();

    void on_restartBtn_clicked();

private:
    Ui::workmodemanager *ui;
    int clickx = 0;
    int clicky = 0;
    QMenu *cMenu;
};

#endif // WORKMODEMANAGER_H
