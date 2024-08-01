#include "usersmanager.h"
#include "ui_usersmanager.h"

usersmanager::usersmanager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::usersmanager)
{
    ui->setupUi(this);
    ui->rightWidget->setVisible(false);
    ui->tableWidget->setAlternatingRowColors(true);//颜色交替显示
    this->showTables();

    //鼠标右键
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    this->cMenu = new QMenu;
    QAction *add = this->cMenu->addAction("新增");
    QAction *repair = this->cMenu->addAction("修改");
    QAction *del = this->cMenu->addAction("删除");
    connect(add,&QAction::triggered,[this](){
        this->on_addusers_clicked();
    });
    connect(repair,&QAction::triggered,[this](){
        this->on_modefyusers_clicked();
    });
    connect(del,&QAction::triggered,[this](){
        this->on_delusers_clicked();
    });


}

usersmanager::~usersmanager()
{
    delete ui;
}

void usersmanager::showTables()
{
    ui->tableWidget->setColumnCount(this->column);
    ui->tableWidget->setHorizontalHeaderLabels({"编号", "姓名","电话", "密码","性别"});
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//自适应
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑

    QString sql = QString ("select * from users");
    query.exec(sql);
    int len=query.size();
    ui->tableWidget->setRowCount(len);
    int i = 0;
    while(query.next()) {
        int j = 0;
        ui->tableWidget->setItem(i, j++, new QTableWidgetItem(query.value("id").toString()));
        ui->tableWidget->setItem(i, j++, new QTableWidgetItem(query.value("name").toString()));
        ui->tableWidget->setItem(i, j++, new QTableWidgetItem(query.value("tel").toString()));
        ui->tableWidget->setItem(i, j++, new QTableWidgetItem(query.value("password").toString()));
        ui->tableWidget->setItem(i, j++, new QTableWidgetItem(query.value("gender").toString()));

        i++;
    }
    for(int c=0;c<len;c++){
        for(int r=0;r<this->column;r++){
            ui->tableWidget->item(c,r)->setTextAlignment(Qt::AlignCenter);
        }
    }
}

void usersmanager::on_addusers_clicked()
{
    loginManager::loginDeal();
    if(loginManager::isLoginFlag==true){
    this->confirm_flag = "新增";
    ui->rightWidget->setVisible(true);
    ui->addusers->setDisabled(true);}
}

void usersmanager::on_confirm_clicked()
{
    int id = ui->idLine->text().toInt();
    QString name = ui->nameLine->text();
    QString tel = ui->telLine->text();
    QString password = ui->passLine->text();
    QString gender = ui->comboBox->currentText();

    //加密
    QByteArray pwd = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5);
    if(this->confirm_flag=="新增"){
        this->sql = QString("insert into users(id,name,tel,password,gender)values(%1,'%2','%3','%4','%5')").arg(id).arg(name).arg(tel).arg( (QString)pwd.toHex() ).arg(gender);
        logsmanager::writeLogs(QString("新增用户：%1").arg(name).toUtf8());
    }
    else if(this->confirm_flag=="修改"){
        if(name=="admin"){
            this->sql = QString("update users set id = '%1', name = '%2', tel = '%3', password = '%4', gender = '%5' where id = %1").arg(id).arg(name).arg(tel).arg(password).arg(gender);
            logsmanager::writeLogs(QString("修改用户：%1 信息").arg(name).toUtf8());
        }
        else {
            this->sql = QString("update users set id = '%1', name = '%2', tel = '%3', password = '%4', gender = '%5' where id = %1").arg(id).arg(name).arg(tel).arg(QString(pwd.toHex())).arg(gender);
            logsmanager::writeLogs(QString("修改用户：%1 信息").arg(name).toUtf8());
        }

    }

    qDebug()<<sql;
    query.exec(this->sql);

    ui->rightWidget->setVisible(false);
    ui->addusers->setDisabled(false);
    ui->modefyusers->setDisabled(false);
    this->showTables();
    this->clear_info();
}

void usersmanager::clear_info() {
    ui->idLine->clear();
    ui->nameLine->clear();
    ui->telLine->clear();
    ui->passLine->clear();
    ui->comboBox->setCurrentIndex(0);
    this->curr_row =-1;
    ui->idLine->setDisabled(false);
}

void usersmanager::on_cancel_clicked()
{
    this->clear_info();
    ui->rightWidget->setVisible(false);
    ui->addusers->setDisabled(false);
    ui->modefyusers->setDisabled(false);
}

void usersmanager::on_delusers_clicked()
{
    loginManager::loginDeal();
    if(loginManager::isLoginFlag==true){
    this->curr_row = ui->tableWidget->currentRow();
    if(this->curr_row<0)
    {
        QMessageBox::critical(this, "操作错误", "请选择要处理的数据！", QMessageBox::Ok);
        return;
    }
//    qDebug()<<curr_row;
    QMessageBox delBox(QMessageBox::Question, "删除提醒", "您确认删除该用户吗?", QMessageBox::Yes | QMessageBox::No);
    delBox.button(QMessageBox::Yes)->setText("确定");
    delBox.setButtonText(QMessageBox::No,"取消");
    if (delBox.exec() == QMessageBox::Yes) {
        QString sql = QString("delete from users where id = %1").arg(ui->tableWidget->item(curr_row, 0)->text().toInt());
        QSqlQuery query;
        query.exec(sql);
    }
    logsmanager::writeLogs(QString("删除用户：%1").arg(ui->tableWidget->item(curr_row, 1)->text()).toUtf8());
    this->showTables();
    this->curr_row=-1;}
}

void usersmanager::on_modefyusers_clicked()
{
    loginManager::loginDeal();
    if(loginManager::isLoginFlag==true){
    this->curr_row = ui->tableWidget->currentRow();
    if(this->curr_row<0)
    {
        QMessageBox::critical(this, "操作错误", "请选择要处理的数据！", QMessageBox::Ok);
        return;
    }
    ui->rightWidget->setVisible(true);
    ui->idLine->setDisabled(true);
    this->confirm_flag = "修改";
    ui->modefyusers->setDisabled(true);
    ui->idLine->setText(ui->tableWidget->item(curr_row, 0)->text());
    ui->nameLine->setText(ui->tableWidget->item(curr_row, 1)->text());
    ui->telLine->setText(ui->tableWidget->item(curr_row, 2)->text());
    ui->passLine->setText(ui->tableWidget->item(curr_row, 3)->text());
    ui->comboBox->setCurrentText(ui->tableWidget->item(curr_row, 4)->text());}
}

void usersmanager::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    this->cMenu->exec(QCursor::pos());
}

void usersmanager::on_toolButton_clicked()
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
