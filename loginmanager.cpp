#include "loginmanager.h"
#include "ui_loginmanager.h"


loginManager::loginManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginManager)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->loginPushButton->setStyleSheet("background-color:#ffffff;"
                                       "border-radius:20px;"
                                       "font:900 normal 19px '黑体'");
    ui->loginPushButton_2->setStyleSheet("background-color:#ffffff;"
                                       "border-radius:20px;"
                                       "font:900 normal 19px '黑体'");
    ui->loginPushButton_3->setStyleSheet("background-color:#ffffff;"
                                       "border-radius:20px;"
                                       "font:900 normal 19px '黑体'");


    ui->pwdLineEdit->setEchoMode(QLineEdit::Password);
    this->generateCaptcha();

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->winMenu=new QMenu;
    QAction *exit=this->winMenu->addAction("关闭");
    QAction *reflu=this->winMenu->addAction("刷新");
    connect(exit,&QAction::triggered,[this](){
        this->close();
    });

    connect(reflu,&QAction::triggered,[this](){
        this->on_loginPushButton_2_clicked();
    });
}

loginManager::~loginManager()
{
    delete ui;
}

//移动窗口
void loginManager::mouseMoveEvent(QMouseEvent *event)
{
//    qDebug()<<"globalPos："<<event->globalPos();//鼠标点距离屏幕的左侧和上方的距离
//    qDebug()<<"localPos："<<event->localPos();//在当前状况下和winpos一样  鼠标距离窗口左侧和上方的距离
//    qDebug()<<"windowPos："<<event->windowPos();
//    qDebug()<<"screenPos："<<event->screenPos();//鼠标点距离屏幕的左侧和上方的距离
//    this->move(event->globalPos());
    int newX=event->globalX()-this->clickX;
    int newY=event->globalY()-this->clickY;
    this->setGeometry(newX,newY,this->width(),this->height());
}

void loginManager::mousePressEvent(QMouseEvent *event)
{
    this->clickX=event->x();
    this->clickY=event->y();
}

bool loginManager::isLoginFlag=false;

void loginManager::loginDeal()
{
    if(!isLoginFlag)
    {
        loginManager *login=new loginManager();
        login->exec();
    }
}

void loginManager::paintEvent(QPaintEvent *event)
{
    QPixmap pixmap("D:/Qt-project/serial-port-server-master/images/666.jpg");
    QPainter painter(this);
    painter.drawPixmap(0, 0, this->width(), this->height(), pixmap);

}

void loginManager::on_loginPushButton_clicked()
{
       QString name = ui->nameLineEdit->text().trimmed();
       QString password = ui->pwdLineEdit->text().trimmed();

       QString input = ui->LineEdit->text().toLower();
       this->correctCaptcha = ui->pushButton_2->text().toLower();

       if(input == correctCaptcha)
       {
           if(name == "admin"){
               this->sql = QString("select * from users where name = '%1' and password = '%2' ").arg(name).arg(password);
           }else {
               //加密
               QByteArray pwd = QCryptographicHash::hash(password.toUtf8(),QCryptographicHash::Md5);
               this->sql = QString("select * from users where name = '%1' and password = '%2' ").arg(name).arg(QString(pwd.toHex()));
           }
           //登录
           //qDebug()<<"开始登录";
           //qDebug()<<this->sql;
           query.exec(this->sql);
           if( query.next() ) {
               qDebug() <<  "登陆成功:" << query.value("name");
               logsmanager::writeLogs(QString("%1登录系统").arg(name).toUtf8());
               accept();
               this->isLoginFlag=true;
               this->close();
           } else {
               QMessageBox myBox(QMessageBox::Question, "警告", "用户名或密码错误，请重新输入！", QMessageBox::Yes);
               myBox.setButtonText(QMessageBox::Yes, "好的");
               myBox.exec();
               ui->nameLineEdit->clear();
               ui->pwdLineEdit->clear();
           }
       }
       else
       {
           //弹出对话框
           QMessageBox myBox(QMessageBox::Question, "警告", "验证码输入错误，请重新输入！", QMessageBox::Yes);
           myBox.setButtonText(QMessageBox::Yes, "好的");
           myBox.exec();
           ui->LineEdit->clear();
           this->generateCaptcha();
       }
}


void loginManager::on_pushButton_clicked()
{
    if (ui->pwdLineEdit->echoMode()==QLineEdit::Password) {
        // 如果当前隐藏密码，则切换为显示密码
        ui->pushButton->setIcon(QIcon(":/images/mimayincang"));
        ui->pwdLineEdit->setEchoMode(QLineEdit::Normal);
    } else {
        // 如果当前显示密码，则切换为隐藏密码
        ui->pwdLineEdit->setEchoMode(QLineEdit::Password);
        ui->pushButton->setIcon(QIcon(":/images/mingwen"));
    }
}

void loginManager::on_loginPushButton_2_clicked()
{
    ui->nameLineEdit->clear();
    ui->pwdLineEdit->clear();
}

void loginManager::on_loginManager_customContextMenuRequested(const QPoint &pos)
{
    this->winMenu->exec(QCursor::pos());
}

void loginManager::on_loginPushButton_3_clicked()
{
    this->close();
}

void loginManager::generateCaptcha()
{
    const QString charSet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    QString captcha = "";
    for (int i = 0; i < 5; ++i) { // 假设验证码长度为5
       captcha += charSet.at(QRandomGenerator::global()->bounded(charSet.size()));
    }
     ui->pushButton_2->setText(captcha);
    //         this->correctCaptcha = captcha.toLower();//不区分大小写
    // 如果需要，还可以将captcha保存为成员变量以便验证
}

void loginManager::on_pushButton_2_clicked()
{
    this->generateCaptcha();
}
