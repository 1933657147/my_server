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

void Widget::resizeEvent(QResizeEvent *event)
{
    this->homep->resize(ui->homeWidget->width(),ui->homeWidget->height());
}

void Widget::setPage(int m)
{
     ui->tabWidget->setCurrentIndex(m);
}

void Widget::on_tabWidget_currentChanged(int index)
{
    this->homep->resize(ui->homeWidget->width(),ui->homeWidget->height());
}
