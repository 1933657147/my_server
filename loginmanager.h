#ifndef LOGINMANAGER_H
#define LOGINMANAGER_H
#include "logsmanager.h"
#include <QDialog>
#include <QPainter>
#include <QPixmap>
#include <QDebug>
#include <QLabel>
#include <QSqlQuery>
#include <QMouseEvent>
#include <QMenu>
#include <QRandomGenerator>
#include <QString>
#include<QCryptographicHash>

namespace Ui {
class loginManager;
}

class loginManager : public QDialog
{
    Q_OBJECT

public:
    explicit loginManager(QWidget *parent = nullptr);
    ~loginManager();
    void paintEvent(QPaintEvent *event);
    static bool isLoginFlag;
    static void loginDeal();
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

private slots:
    void on_loginPushButton_clicked();

    void on_pushButton_clicked();

    void on_loginPushButton_2_clicked();

    void on_loginManager_customContextMenuRequested(const QPoint &pos);

    void on_loginPushButton_3_clicked();

    void generateCaptcha();//验证码

    void on_pushButton_2_clicked();

private:
    Ui::loginManager *ui;
    QSqlQuery query;
    int clickX=0;
    int clickY=0;
    QMenu *winMenu;
    QString correctCaptcha;
    QString sql;

};

#endif // LOGINMANAGER_H
