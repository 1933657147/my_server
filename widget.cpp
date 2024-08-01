#include "widget.h"
#include "ui_widget.h"
#include "loginmanager.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->homep=new homepage(ui->homeWidget);
//    homep->move(10,10);
    //home的大小设置为ui->homewidget
    homep->resize(ui->homeWidget->width(),ui->homeWidget->height());
//    homepage homep;

//    connect(homep,&homepage::firstSignal,[this](){

//        ui->tabWidget->setCurrentIndex(1);
////        homep->close();
//    });
//    homep->setHidden(true);
    connect(homep,SIGNAL(firstSignal(int)),this,SLOT(setPage(int)));

}

Widget::~Widget()
{
    delete ui;
}

//当窗口大小调整时，调整homep的大小以匹配ui->homewidget
void Widget::resizeEvent(QResizeEvent *event)
{
    this->homep->resize(ui->homeWidget->width(),ui->homeWidget->height());
}

//更改QTabWidget的当前页面
void Widget::setPage(int m)
{
     ui->tabWidget->setCurrentIndex(m);
}

//当QTabWidget的当前页改变时，调整homep的大小以匹配ui->homewidget
void Widget::on_tabWidget_currentChanged(int index)
{
    this->homep->resize(ui->homeWidget->width(),ui->homeWidget->height());
}
