#include "homepage.h"
#include "ui_homepage.h"
#include<QPainter>
#include<QDebug>
#include<QDesktopServices>
#include<QUrl>
homepage::homepage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::homepage)
{
    ui->setupUi(this);



}
void homepage::paintEvent(QPaintEvent *event)
{
    QPixmap pixmap("D:/Qt-project/serial-port-server-master/images/wan.jpeg");
    QPainter painter(this);
    painter.drawPixmap(0, 0, this->width(), this->height(), pixmap);
}

homepage::~homepage()
{
    delete ui;
}

void homepage::on_toolButton_clicked()
{
    emit this->firstSignal(1);
}

void homepage::on_toolButton_2_clicked()
{
    emit this->firstSignal(2);
}

void homepage::on_toolButton_4_clicked()
{
     emit this->firstSignal(3);
}

void homepage::on_toolButton_3_clicked()
{
    emit this->firstSignal(4);
}


void homepage::on_pushButton_clicked()
{
    QDesktopServices::openUrl(QUrl("https://leetcode.cn/studyplan/top-100-liked/"));

}
