#include "homepage.h"
#include "ui_homepage.h"
#include<QPainter>
#include<QDebug>
#include<QDesktopServices>//提供打开URL的功能
#include<QUrl>//处理URL
homepage::homepage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::homepage)
{
    ui->setupUi(this);



}
//事件处理函数
void homepage::paintEvent(QPaintEvent *event)
{
    //加载绝对路径下的图片
    QPixmap pixmap("D:/Qt-project/serial-port-server-master/images/wan.jpeg");

    //初始化画家
    QPainter painter(this);
    //将图片绘制到窗口上，缩放图片用于填充窗口的大小
    painter.drawPixmap(0, 0, this->width(), this->height(), pixmap);
}

homepage::~homepage()
{
    delete ui;
}


//每个槽函数都发射信号，并且传递参数，参数表示不同的操作
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
    //按钮点击事件，打开指定的URL
    QDesktopServices::openUrl(QUrl("https://leetcode.cn/studyplan/top-100-liked/"));

}
