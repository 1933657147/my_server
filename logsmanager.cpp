#include "logsmanager.h"
#include "loginmanager.h"
#include "ui_logsmanager.h"

logsmanager::logsmanager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::logsmanager)
{
    ui->setupUi(this);
    //初始化时读取当前页面日志
    this->readLogs();
    //设置分页和当前页面
    this->pagesize = 20;
    this->currpage = 1;
    //显示日志内容
    this->showLogs();
    //设置界面中每页的日志数
    ui->spinBox->setValue(this->pagesize);
    //计算总页数
    this->totalpage = ceil(this->logsList.size() / (double)this->pagesize);

}

logsmanager::~logsmanager()
{
    delete ui;
}


//指定加载日志文件的路径
QFile logsmanager::file("C:\ProgramData\DisplaySessionContainer1.log");

//将日志数据追加到日志文件中，包含时间戳。
void logsmanager::writeLogs(QByteArray data) {
    if(file.open(QIODevice::Append)) {
        QString logInfo = QString("%1 >> %2\n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")).arg(QString(data));
        file.write(logInfo.toUtf8());
        file.close();
    }
}

//读取日志文件的内容，并且按行分割
void logsmanager::readLogs() {
    if (this->file.exists() && this->file.open(QIODevice::ReadOnly)) {
        this->logsList = QString::fromUtf8(file.readAll()).split("\n");
        this->file.close();
    }
}

//日志显示与分页
void logsmanager::showLogs() {
    //清空日志视图并且重新加载当前页的日志
    ui->logView->clear();

    if(this->logsList.size() % this->pagesize == 0)
    {
        this->totalpage = this->logsList.size() / (double)this->pagesize -1;
    }
    else {
        this->totalpage = ceil(this->logsList.size() / (double)this->pagesize);
    }
    ui->logView->setRowCount(this->pagesize);
    ui->logView->setColumnCount(1);
    ui->logView->horizontalHeader()->setVisible(false);// 隐藏水平表头
    ui->logView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//自适应
    ui->logView->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑
    ui->pageNum->setText(QString("%1 / %2").arg(this->currpage).arg(this->totalpage));
    int row = 0;
    for(int i = this->pagesize * (this->currpage - 1); i <= this->pagesize * this->currpage - 1 && i < this->logsList.size(); i++) {
        ui->logView->setItem(row++, 0, new QTableWidgetItem(this->logsList[i]));
    }
}

//按钮点击事件：下一页
void logsmanager::on_nextBtn_clicked()
{
    if (++this->currpage==this->totalpage)
    {
        ui->nextBtn->setDisabled(true);
    }
    else {
        ui->nextBtn->setDisabled(false);
        ui->prevBtn->setDisabled(false);
    }
    ui->comboBox->setCurrentIndex(this->currpage-1);
    this->showLogs();
}

//按钮点击事件：上一页
void logsmanager::on_prevBtn_clicked()
{
    if (--this->currpage==1)
    {
        ui->prevBtn->setDisabled(true);
    }
    else {
        ui->prevBtn->setDisabled(false);
        ui->nextBtn->setDisabled(false);
    }
    ui->comboBox->setCurrentIndex(this->currpage-1);
    this->showLogs();
}

//按钮点击事件：更新日志
void logsmanager::on_updateBtn_clicked()
{
    loginManager::loginDeal();
    if(loginManager::isLoginFlag==true){
    this->currpage = 1;
    ui->comboBox->setCurrentIndex(this->currpage-1);
    ui->prevBtn->setDisabled(true);
    ui->nextBtn->setDisabled(false);
    this->readLogs();
    this->showLogs();
    }
}

//处理页码选择变化时，上下页按钮的点击
void logsmanager::on_comboBox_currentIndexChanged(int index)
{
    if(ui->comboBox->currentText()!=NULL)
    {
        this->currpage=index+1;
        this->showLogs();
        if(this->currpage==1)
        {
            ui->prevBtn->setDisabled(true);
            ui->nextBtn->setDisabled(false);
        }
        else if (this->currpage==this->totalpage) {
            ui->nextBtn->setDisabled(true);
            ui->prevBtn->setDisabled(false);
        }
        else {
            ui->prevBtn->setDisabled(false);
            ui->nextBtn->setDisabled(false);
        }
    }
}

//处理每页日志数变化
void logsmanager::on_spinBox_valueChanged(int arg1)
{
    this->pagesize=arg1;
    this->totalpage=ceil(this->logsList.size()/this->pagesize);
    this->showLogs();
    if(!ui->spinBox->text().isEmpty()){
        if(ui->spinBox->text().toInt()>0){
            ui->comboBox->clear();
            for(int i=1;i<=this->totalpage;i++)
            {
                ui->comboBox->addItem(QString("第%1页").arg(i));
            }
        }
        else {
            qDebug()<<ui->spinBox->text();
        }
    }
    else {
        return;qDebug()<<ui->spinBox->text();
    }
}
//退出登录
void logsmanager::on_toolButton_clicked()
{
    // 弹出一个确认对话框
    if(loginManager::isLoginFlag == true)
    {
           QMessageBox::StandardButton reply;
           reply = QMessageBox::question(this, "退出登录", "你确定要退出登录吗？",
                        QMessageBox::Yes | QMessageBox::No);

           if (reply == QMessageBox::Yes)
           {
               loginManager::isLoginFlag = false;
           }
     }
}
