#include "workmodemanager.h"
#include "datamanager.h"
#include "mytcpclient.h"
#include "ui_workmodemanager.h"

workmodemanager::workmodemanager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::workmodemanager)
{
    ui->setupUi(this);
    ui->protocolBox->setDisabled(true);

    ui->verticalLayoutWidge->setVisible(false);
    ui->tableWidget->setAlternatingRowColors(true);//颜色交替显示
    this->showTables();



    //鼠标右键
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    this->cMenu = new QMenu;
    QAction *add = this->cMenu->addAction("新增");
    QAction *repair = this->cMenu->addAction("修改");
    QAction *del = this->cMenu->addAction("删除");
    connect(add,&QAction::triggered,[this](){
        this->on_addBtn_clicked();
    });
    connect(repair,&QAction::triggered,[this](){
        this->on_modifyBtn_clicked();
    });
    connect(del,&QAction::triggered,[this](){
        this->on_delBtn_clicked();
    });
}

workmodemanager::~workmodemanager()
{
    delete ui;
}

void workmodemanager::showTables()
{
    ui->tableWidget->setColumnCount(this->column);
    ui->tableWidget->setHorizontalHeaderLabels({"编号", "工作模式", "TCP数据", "本地端口", "回车处理", "换行处理", "链接数", "协议", "对端IP", "对端PORT", "链接方式", "断开连接", "空闲时间", "操作"});
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//自适应
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑

    QString sql = QString("select * from workmode");
    QSqlQuery query;
    if(!query.exec(sql)){
        qWarning()<<"SQL Error"<<query.lastError();
    }

    int len = query.size();
    ui->tableWidget->setRowCount(len);

    int i = 0;
    while(query.next()) {
        int j = 0;
        //int id = query.value("id").toInt();
        ui->tableWidget->setItem(i, j++, new QTableWidgetItem(query.value("id").toString()));
        ui->tableWidget->setItem(i, j++, new QTableWidgetItem(query.value("workmode").toString()));
        ui->tableWidget->setItem(i, j++, new QTableWidgetItem(query.value("tcp_datamode").toString()));
        ui->tableWidget->setItem(i, j++, new QTableWidgetItem(query.value("port_no").toString()));
        ui->tableWidget->setItem(i, j++, new QTableWidgetItem(query.value("cr_explained").toString()));
        ui->tableWidget->setItem(i, j++, new QTableWidgetItem(query.value("lf_explained").toString()));
        ui->tableWidget->setItem(i, j++, new QTableWidgetItem(query.value("links_num").toString()));
        ui->tableWidget->setItem(i, j++, new QTableWidgetItem(query.value("protocol").toString()));
        ui->tableWidget->setItem(i, j++, new QTableWidgetItem(query.value("peerhost").toString()));
        ui->tableWidget->setItem(i, j++, new QTableWidgetItem(query.value("peerport").toString()));
        ui->tableWidget->setItem(i, j++, new QTableWidgetItem(query.value("connect").toString()));
        ui->tableWidget->setItem(i, j++, new QTableWidgetItem(query.value("disconnect").toString()));
        ui->tableWidget->setItem(i, j++, new QTableWidgetItem(query.value("freetime").toString()));
        //为每一行添加操作按钮
        // 创建一个空的 QWidget   把水平部件放进widget
        QWidget *widget = new QWidget;
        QHBoxLayout *hLayout = new QHBoxLayout(widget);
        QPushButton *restartBtn = new QPushButton("重启");
        hLayout->addWidget(restartBtn);
        //设置水平布局的margin为0，让按钮正常显示处理
        hLayout->setMargin(0);
        //把widget添加到单元格中
        ui->tableWidget->setCellWidget(i++, j, widget);
        // 设置单元格宽度
        ui->tableWidget->setColumnWidth(j, 50);
        connect(restartBtn, &QPushButton::clicked, [this]() {
            //this->curr_row = ui->tableWidget->currentRow();
            //qDebug()<<QString("将要重启%1").arg(ui->tableWidget->item(curr_row, 0)->text()).toUtf8();
            qDebug()<<"重启按钮被点击";
        });

    }
    for(int c=0;c<len;c++){
        for(int r=0;r<this->column-1;r++){
            ui->tableWidget->item(c,r)->setTextAlignment(Qt::AlignCenter);
        }
    }
}


void workmodemanager::on_workmodeBox_currentTextChanged(const QString &arg1)
{
    QString curr_workmode = arg1;
    if(curr_workmode == "TCP Server") {
        ui->protocolBox->setCurrentText("1");
        ui->peerportBox->setDisabled(true);
        ui->links_numBox->setDisabled(false);
        ui->peerhostLineEdit->setDisabled(true);
        ui->freetimeBox->setDisabled(true);
    } else if (curr_workmode == "TCP Client") {
        ui->protocolBox->setCurrentText("0");
        ui->peerportBox->setDisabled(false);
        ui->peerhostLineEdit->setDisabled(false);
        ui->links_numBox->setDisabled(true);
        ui->freetimeBox->setDisabled(false);
    } else if (curr_workmode == "UDP") {
        ui->protocolBox->setCurrentText("2");
        ui->peerportBox->setDisabled(false);
        ui->peerhostLineEdit->setDisabled(false);
        ui->links_numBox->setDisabled(true);
        ui->freetimeBox->setDisabled(false);
    }
}

void workmodemanager::on_addBtn_clicked()
{
    loginManager::loginDeal();
    if(loginManager::isLoginFlag==true){
    this->confirm_flag = "新增";
    ui->verticalLayoutWidge->setVisible(true);
    ui->addBtn->setDisabled(true);
    this->unusedPort_no();}
}

void workmodemanager::on_modifyBtn_clicked()
{
    loginManager::loginDeal();
    if(loginManager::isLoginFlag==true){
    this->curr_row = ui->tableWidget->currentRow();
    if(this->curr_row<0)
    {
        QMessageBox::critical(this, "操作错误", "请选择要处理的数据！", QMessageBox::Ok);
        return;
    }
    this->confirm_flag = "修改";
    this->unusedPort_no();
    ui->verticalLayoutWidge->setVisible(true);
    ui->modifyBtn->setDisabled(true);
    ui->idLine->setDisabled(true);
    ui->idLine->setText(ui->tableWidget->item(curr_row, 0)->text());
    ui->workmodeBox->setCurrentText(ui->tableWidget->item(curr_row, 1)->text()) ;
    ui->tcp_datamodeBox->setCurrentText(ui->tableWidget->item(curr_row, 2)->text());
    ui->port_noBox->setCurrentText(ui->tableWidget->item(curr_row, 3)->text());
    ui->cr_explainedBox->setCurrentText(ui->tableWidget->item(curr_row, 4)->text());
    ui->lf_explainedBox->setCurrentText(ui->tableWidget->item(curr_row, 5)->text());
    ui->links_numBox->setValue(ui->tableWidget->item(curr_row, 6)->text().toInt());
    ui->protocolBox->setCurrentText(ui->tableWidget->item(curr_row, 7)->text());
    ui->peerhostLineEdit->setText(ui->tableWidget->item(curr_row, 8)->text());
    ui->peerportBox->setValue(ui->tableWidget->item(curr_row, 9)->text().toInt());
    ui->connectBox->setCurrentText(ui->tableWidget->item(curr_row, 10)->text());
    ui->disconnectBox->setCurrentText(ui->tableWidget->item(curr_row, 11)->text());
    ui->freetimeBox->setValue(ui->tableWidget->item(curr_row, 12)->text().toInt());}
}

void workmodemanager::on_certainBtn_clicked()
{
    ui->verticalLayoutWidge->setVisible(true);
    int id =ui->idLine->text().toInt();
    QString workmode = ui->workmodeBox->currentText();
    QString tcp_datamode = ui->tcp_datamodeBox->currentText();
    int port_no = ui->port_noBox->currentText().toInt();
    QString cr = ui->cr_explainedBox->currentText();
    QString lf = ui->lf_explainedBox->currentText();
    int links_num = ui->links_numBox->value();
    int protocol = ui->protocolBox->currentText().toInt();
    QString peerhost = ui->peerhostLineEdit->text();
    int peerport = ui->peerportBox->value();
    QString connect = ui->connectBox->currentText();
    QString disconnect = ui->disconnectBox->currentText();
    int freetime = ui->freetimeBox->value();

    if(this->confirm_flag =="新增") {
   //     this->sql = QString("insert into workmode(workmode, tcp_datamode, port_no, cr_explained, lf_explained, links_num, protocol, peerhost, peerport, connect, disconnect, freetime,id) values ('%1', '%2', %3, '%4', '%5', %6, %7, '%8', %9, '%10', '%11', %12,%13)").arg(workmode).arg(tcp_datamode).arg(port_no).arg(cr).arg(lf).arg(links_num).arg(protocol).arg(peerhost).arg(peerport).arg(connect).arg(disconnect).arg(freetime).arg(id);
   //     logsmanager::writeLogs(QString("新增工作模式编号：%1").arg(ui->tableWidget->item(curr_row, 0)->text()).toUtf8());


        this->sql = QString("INSERT INTO workmode(workmode, tcp_datamode, port_no, cr_explained, lf_explained, links_num, protocol, peerhost, peerport, connect, disconnect, freetime, id) VALUES ('%1', '%2', %3, '%4', '%5', %6, %7, '%8', %9, '%10', '%11', %12, %13)")
                        .arg(workmode)
                        .arg(tcp_datamode)
                        .arg(port_no) // 假设port_no是一个整数或可以转换为整数的类型
                        .arg(cr)
                        .arg(lf)
                        .arg(links_num) // 假设links_num也是一个整数或可以转换为整数的类型
                        .arg(protocol) // 如果protocol是一个字符串，确保它已经被适当地转义
                        .arg(peerhost)
                        .arg(peerport) // 假设peerport是一个整数或可以转换为整数的类型
                        .arg(connect)
                        .arg(disconnect)
                        .arg(freetime)
                        .arg(id); // 假设id是一个整数或可以转换为整数的类型




    } else if (this->confirm_flag=="修改") {
        //int curr_id = ui->tableWidget->item(this->curr_row, 0)->text().toInt(); // 获取当前修改信息行的id
        this->sql = QString("update workmode set workmode = '%1', tcp_datamode = '%2', port_no = %3, cr_explained = '%4', lf_explained = '%5', links_num = %6, protocol = %7, peerhost = '%8', peerport = %9, connect = '%10', disconnect = '%11', freetime = %12 where id = %13").arg(workmode).arg(tcp_datamode).arg(port_no).arg(cr).arg(lf).arg(links_num).arg(protocol).arg(peerhost).arg(peerport).arg(connect).arg(disconnect).arg(freetime).arg(id);
        logsmanager::writeLogs(QString("修改工作模式编号：%1 信息").arg(ui->tableWidget->item(curr_row, 0)->text()).toUtf8());
    }
    qDebug()<<this->sql;
    query.exec(sql);
    this->clear_info();
    this->showTables();
}

void workmodemanager::clear_info()
{
    ui->port_noBox->clear();
    ui->idLine->clear();
    ui->cr_explainedBox->setCurrentText("");
    ui->lf_explainedBox->setCurrentText("");
    ui->links_numBox->setValue(0);
    ui->protocolBox->setCurrentText("");
    ui->peerportBox->setValue(0);
    ui->peerhostLineEdit->setText("");
    ui->connectBox->setCurrentText("");
    ui->disconnectBox->setCurrentText("");
    ui->freetimeBox->setValue(0);

    ui->verticalLayoutWidge->setVisible(false);
    ui->addBtn->setDisabled(false);
    ui->modifyBtn->setDisabled(false);
    ui->idLine->setDisabled(false);
    this->curr_row = -1;
}

void workmodemanager::on_delBtn_clicked()
{
    loginManager::loginDeal();
    if(loginManager::isLoginFlag==true){
    this->curr_row = ui->tableWidget->currentRow();
    if(this->curr_row<0)
    {
        QMessageBox::critical(this, "操作错误", "请选择要处理的数据！", QMessageBox::Ok);
        return;
    }
    QMessageBox delBox(QMessageBox::Question, "删除提醒", "您确认删除该信息吗?", QMessageBox::Yes | QMessageBox::No);
    delBox.button(QMessageBox::Yes)->setText("确定");
    delBox.setButtonText(QMessageBox::No,"取消");
    if ( delBox.exec() == QMessageBox::Yes) {
        this->sql = QString("delete from workmode where id = %1").arg(ui->tableWidget->item(curr_row, 0)->text().toInt());
        query.exec(sql);
    }
    logsmanager::writeLogs(QString("删除工作模式编号：%1").arg(ui->tableWidget->item(curr_row, 0)->text()).toUtf8());
    this->showTables();
    this->curr_row = -1;}
}

void workmodemanager::on_cancelBtn_clicked()
{
    ui->addBtn->setDisabled(false);
    ui->modifyBtn->setDisabled(false);
    ui->verticalLayoutWidge->setVisible(false);
    this->clear_info();
}

void workmodemanager::unusedPort_no()
{
    QString port_no_sql = QString("SELECT serial_port.port_no FROM serial_port LEFT JOIN workmode ON serial_port.port_no = workmode.port_no WHERE workmode.port_no IS NULL");
//    if(this->confirm_flag=="修改")
//    {

//    }

  //QString port_no_sql = QString("SELECT port_no FROM serial_port ");

    query.exec(port_no_sql);
    qDebug()<<port_no_sql;
    while (query.next())
    {
        ui->port_noBox->addItem(query.value("port_no").toString());
    }
}

void workmodemanager::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    this->cMenu->exec(QCursor::pos());
}



void workmodemanager::on_toolButton_clicked()
{
    // 弹出一个确认对话框
    if(loginManager::isLoginFlag == true)
    {
           QMessageBox::StandardButton reply;
           reply = QMessageBox::question(this, "退出登录", "你确定要退出登录吗？",
                                        QMessageBox::Yes | QMessageBox::No);

           //loginManager::loginDeal();
           if (reply == QMessageBox::Yes)
           {
               loginManager::isLoginFlag = false;
           }
     }
}

void workmodemanager::on_restartBtn_clicked()
{


    dataManager data;
    data.connect_db();

    for(auto com : serialPortMap) {
            com->close();
    //        com->destroyed();
    //        delete com;
        }

    mytcpclient::startUartConfig();


//    for(auto com : serialPortMap) {
//        com->close();
////        com->destroyed();
////       delete com;
//    }
//    serialPortMap.clear();



////    //调试串口
//    mytcpclient mytcp_c;
//    mytcp_c.startUartConfig();
}
