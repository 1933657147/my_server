#include "uartmanager.h"
#include "ui_uartmanager.h"
#include <QSerialPortInfo>

uartmanager::uartmanager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::uartmanager)
{
    ui->setupUi(this);
    ui->rightWidget->setVisible(true);//隐藏右侧小部件
    ui->tableWidget->setAlternatingRowColors(true);//颜色交替显示
    this->showTables();//显示表格内容
    QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();//获取串口，并且添加到下拉框
    for(auto port : portList) {
        if(!port.portName().contains("NULL")) {
            ui->com_nmCbB->addItem(port.portName());
        }
    }


    //鼠标右键菜单连接相应的槽函数
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    this->cMenu = new QMenu;
    QAction *add = this->cMenu->addAction("新增");
    QAction *repair = this->cMenu->addAction("修改");
    QAction *del = this->cMenu->addAction("删除");
    connect(add,&QAction::triggered,[this](){
        this->on_addBtn_clicked();
    });
    connect(repair,&QAction::triggered,[this](){
        this->on_editBtn_clicked();
    });
    connect(del,&QAction::triggered,[this](){
        this->on_deleteBtn_clicked();
    });
}

uartmanager::~uartmanager()
{
    delete ui;
}
//显示表格数据
void uartmanager::showTables() {

    ui->tableWidget->setColumnCount(this->column);
    ui->tableWidget->setHorizontalHeaderLabels({"编号", "端口", "串口号", "串口名", "波特率", "数据位", "停止位", "校验位", "流控制", "接收空闲时间", "发送空闲时间"});
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//自适应

    QString sql = QString("select * from  serial_config");
    query.exec(sql);
    int len=query.size();
    ui->tableWidget->setRowCount(len);

    int i = 0;
    while(query.next()) {
        int j = 0;
        ui->tableWidget->setItem(i, j++, new QTableWidgetItem(QString::number(query.value("id").toInt())));
        ui->tableWidget->setItem(i, j++, new QTableWidgetItem(QString::number(query.value("port_no").toInt())));
        ui->tableWidget->setItem(i, j++, new QTableWidgetItem(QString::number(query.value("com_no").toInt())));
        ui->tableWidget->setItem(i, j++, new QTableWidgetItem(query.value("com_nm").toString()));
        ui->tableWidget->setItem(i, j++, new QTableWidgetItem(QString::number(query.value("baudrate").toInt())));
        ui->tableWidget->setItem(i, j++, new QTableWidgetItem(QString::number(query.value("databit").toInt())));
        ui->tableWidget->setItem(i, j++, new QTableWidgetItem(QString::number(query.value("stopbit").toInt())));
        ui->tableWidget->setItem(i, j++, new QTableWidgetItem(query.value("checkbit").toString()));
        ui->tableWidget->setItem(i, j++, new QTableWidgetItem(query.value("flowcontrol").toString()));
        ui->tableWidget->setItem(i, j++, new QTableWidgetItem(QString::number(query.value("rx_time").toInt())));
        ui->tableWidget->setItem(i++, j, new QTableWidgetItem(QString::number(query.value("tx_time").toInt())));
    }
    for(int c=0;c<len;c++){
        for(int r=0;r<this->column;r++){
            ui->tableWidget->item(c,r)->setTextAlignment(Qt::AlignCenter);
        }
    }
//    ui->tableWidget->setColumnHidden(0, true);  //设置隐藏列
}

//检查用户是否登录，获取当前选择的行，弹出确认对话框询问是否删除所选中的记录，确认后删除数据，并且记录日志
void uartmanager::on_deleteBtn_clicked()
{
    loginManager::loginDeal();
    if(loginManager::isLoginFlag==true){
    this->curr_row = ui->tableWidget->currentRow();
    if(this->curr_row<0)
    {
        QMessageBox::critical(this, "操作错误", "请选择要处理的数据！", QMessageBox::Ok);
        return;
    }
    QMessageBox  delBox(QMessageBox::Question, "删除提醒", QString("你确认要删除%1吗？").arg( ui->tableWidget->item( this->curr_row, 3)->text()), QMessageBox::Yes | QMessageBox::No);
    delBox.button(QMessageBox::Yes)->setText("确定");
    delBox.setButtonText(QMessageBox::No,"取消");
    if (delBox.exec() == QMessageBox::Yes) {
        QString sql = QString("delete from serial_port where id = %1").arg(ui->tableWidget->item(curr_row, 0)->text().toInt());
        QSqlQuery query;
        query.exec(sql);

    logsmanager::writeLogs(QString("删除串口：%1").arg(ui->tableWidget->item(curr_row, 2)->text()).toUtf8());
    }
    this->showTables();
    this->curr_row=-1;
    }
}


void uartmanager::on_editBtn_clicked()
{
    loginManager::loginDeal();
    //判断是否已经登录
    if(loginManager::isLoginFlag==true){
    //获取当前选中的行
    this->curr_row = ui->tableWidget->currentRow();
    this->confirm_flag = "修改";
    //验证选择
    if(this->curr_row<0)
    {
        QMessageBox::critical(this, "操作错误", "请选择要处理的数据！", QMessageBox::Ok);
        return;
    }
    //设置编辑模式
    ui->rightWidget->setVisible(true);
    ui->editBtn->setDisabled(true);
    ui->idLine->setDisabled(true);
    //填充编辑数据
    ui->idLine->setText( ui->tableWidget->item(this->curr_row,0)->text());
    ui->port_noSp->setValue(ui->tableWidget->item(this->curr_row, 1)->text().toInt());
    ui->com_noSp->setValue(ui->tableWidget->item(this->curr_row, 2)->text().toInt() );
    ui->com_nmCbB->setCurrentText(ui->tableWidget->item(this->curr_row, 3)->text());
    ui->baudrateCbB->setCurrentText(ui->tableWidget->item(this->curr_row, 4)->text());
    ui->databitCbB->setCurrentText(ui->tableWidget->item(this->curr_row, 5)->text());
    ui->stopbitCbB->setCurrentText(ui->tableWidget->item(this->curr_row, 6)->text());
    ui->checkbitCbB->setCurrentText( ui->tableWidget->item(this->curr_row, 7)->text());
    ui->flowcontrolLe->setText(ui->tableWidget->item(this->curr_row, 8)->text());
    ui->rx_timeSp->setValue(ui->tableWidget->item(this->curr_row, 9)->text().toInt());
    ui->tx_timeSp->setValue(ui->tableWidget->item(this->curr_row, 10)->text().toInt());}
}

void uartmanager::on_saveBtn_clicked()
{
    //获取输入的数据
    int id =ui->idLine->text().toInt();
    int port_no =  ui->port_noSp->value();
    int com_no =  ui->com_noSp->value();
    QString com_nm = ui->com_nmCbB->currentText();
    int baudrate = ui->baudrateCbB->currentText().toInt();
    int databit = ui->databitCbB->currentText().toInt();
    int stopbit = ui->stopbitCbB->currentText().toInt();
    QString checkbit = ui->checkbitCbB->currentText();
    QString flowcontrol =  ui->flowcontrolLe->text();
    int rx_time =  ui->rx_timeSp->text().toInt();
    int tx_time =  ui->tx_timeSp->text().toInt();
    //处理新增，并且记录日志
    if(this->confirm_flag=="新增"){
        this->sql = QString("insert into serial_port(port_no,com_no,com_nm,baudrate,databit,stopbit,checkbit,flowcontrol,rx_time,tx_time,id) values(%1,%2,'%3',%4,%5,%6,'%7','%8',%9,%10,%11)").arg(port_no).arg(com_no).arg(com_nm).arg(baudrate).arg(databit).arg(stopbit).arg(checkbit).arg(flowcontrol).arg(rx_time).arg(tx_time).arg(id);
        logsmanager::writeLogs(QString("新增串口：%1").arg(ui->tableWidget->item(curr_row, 2)->text()).toUtf8());
    }
    //处理修改，记录日志
    else if(this->confirm_flag=="修改"){
        this->sql = QString("update serial_port set port_no = %1, com_no= %2, com_nm='%3', baudrate=%4, databit=%5,  stopbit=%6,  checkbit='%7', flowcontrol='%8',  rx_time=%9, tx_time=%10 where id = %11").arg(port_no).arg(com_no).arg(com_nm).arg(baudrate).arg(databit).arg(stopbit).arg(checkbit).arg(flowcontrol).arg(rx_time).arg(tx_time).arg(ui->tableWidget->item(this->curr_row, 0)->text().toInt());
        logsmanager::writeLogs(QString("修改串口：%1 信息").arg(ui->tableWidget->item(curr_row, 2)->text()).toUtf8());
    }

    //执行SQL
    query.exec(sql);
    //更新界面
    this->showTables();
    //恢复添加和编辑按钮的状态。隐藏编辑窗口
    ui->addBtn->setDisabled(false);
    ui->editBtn->setDisabled(false);
    ui->rightWidget->setVisible(false);
    //清空编辑控件的内容
    this->clear_info();
}

void uartmanager::on_addBtn_clicked()
{
    //确保用户已经登录，将 this->confirm_flag 设置为 "新增"，禁用“添加”按钮 (ui->addBtn->setDisabled(true)) 并显示编辑窗口 (ui->rightWidget->setVisible(true))，以便用户输入新的数据
    loginManager::loginDeal();
    if(loginManager::isLoginFlag==true){
    this->confirm_flag = "新增";
    ui->addBtn->setDisabled(true);
    ui->rightWidget->setVisible(true);}
}

void uartmanager::clear_info()
{
    //清空控件内容，将所有控件恢复到初始状态
    ui->idLine->clear();
    //重置当前行
    ui->port_noSp->setValue(0);
    ui->com_noSp->setValue(0);
    ui->baudrateCbB->setCurrentIndex(0);
    ui->databitCbB->setCurrentIndex(0);
    ui->stopbitCbB->setCurrentIndex(0);
    ui->checkbitCbB->setCurrentIndex(0);
    ui->flowcontrolLe->setText("");
    ui->rx_timeSp->setValue(0);
    ui->tx_timeSp->setValue(0);
    this->curr_row =-1;
    //重新启用ID输入框
    ui->idLine->setDisabled(false);
}

void uartmanager::on_cancelBtn_clicked()
{
    //恢复按钮状态：启用添加和编辑按钮
    ui->addBtn->setDisabled(false);
    ui->editBtn->setDisabled(false);
     //隐藏编辑窗口
    ui->rightWidget->setVisible(false);
     //清空编辑控件的内容
    this->clear_info();
}

//右键菜单
void uartmanager::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    //显示自定义的右键菜单
    this->cMenu->exec(QCursor::pos());
}

//退出登录
void uartmanager::on_toolButton_clicked()
{
    // 弹出一个确认对话框，确保用户已经登录
    if(loginManager::isLoginFlag == true)
    {
           QMessageBox::StandardButton reply;
           reply = QMessageBox::question(this, "退出登录", "你确定要退出登录吗？",
               QMessageBox::Yes | QMessageBox::No);
            //更新登录状态==将标志位设置为false表示已经退出登录
           if (reply == QMessageBox::Yes)
           {
               loginManager::isLoginFlag = false;
           }
     }
}
