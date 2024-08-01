#include "loginmanager.h"
#include "ui_loginmanager.h"


loginManager::loginManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginManager)
{
    ui->setupUi(this);
    //设置窗口为无边窗口
    this->setWindowFlags(Qt::FramelessWindowHint);
    //为按钮设置自定义样式，包括背景色，边框半径，字体
    ui->loginPushButton->setStyleSheet("background-color:#ffffff;"
                                       "border-radius:20px;"
                                       "font:900 normal 19px '黑体'");
    ui->loginPushButton_2->setStyleSheet("background-color:#ffffff;"
                                       "border-radius:20px;"
                                       "font:900 normal 19px '黑体'");
    ui->loginPushButton_3->setStyleSheet("background-color:#ffffff;"
                                       "border-radius:20px;"
                                       "font:900 normal 19px '黑体'");

    //将密码输入框的回显模式设置为密码模式，隐藏输入的字符
    ui->pwdLineEdit->setEchoMode(QLineEdit::Password);
    //调用自定义函数生成的验证码
    this->generateCaptcha();
    //启用自定义上下文菜单
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->winMenu=new QMenu;
    //添加关闭和刷新菜单选项
    QAction *exit=this->winMenu->addAction("关闭");
    QAction *reflu=this->winMenu->addAction("刷新");
    //为菜单选项连接槽函数
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

//窗口移动事件：记录鼠标点击的位置，用于窗口的移动
void loginManager::mouseMoveEvent(QMouseEvent *event)
{
    int newX=event->globalX()-this->clickX;
    int newY=event->globalY()-this->clickY;
    this->setGeometry(newX,newY,this->width(),this->height());
}
//窗口移动事件，根据鼠标移动的位置调整窗口的位置，实现拖动窗口的效果
void loginManager::mousePressEvent(QMouseEvent *event)
{
    this->clickX=event->x();
    this->clickY=event->y();
}

//isLoginFlag是LoginManager类的一个静态成员变量，初始化它，用于标记是否以及登录成功，防止重复登录
bool loginManager::isLoginFlag=false;

//登录处理：如果未登录，创建并且执行一个新的登录对话框
void loginManager::loginDeal()
{
    if(!isLoginFlag)
    {
        loginManager *login=new loginManager();
        login->exec();
    }
}

//绘制事件：重写绘制事件函数，用于绘制背景图片
void loginManager::paintEvent(QPaintEvent *event)
{
    QPixmap pixmap("D:/Qt-project/serial-port-server-master/images/666.jpg");
    QPainter painter(this);
    painter.drawPixmap(0, 0, this->width(), this->height(), pixmap);

}

//登录按钮处理
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
               this->isLoginFlag=true;//设置登录标志
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

//密码显示/隐藏切换，当点击按钮时实现切换
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

//清除输入框的内容
void loginManager::on_loginPushButton_2_clicked()
{
    ui->nameLineEdit->clear();
    ui->pwdLineEdit->clear();
}

//自定义上下文菜单：显示自定义的上下文菜单
void loginManager::on_loginManager_customContextMenuRequested(const QPoint &pos)
{
    this->winMenu->exec(QCursor::pos());
}

//关闭当前窗口
void loginManager::on_loginPushButton_3_clicked()
{
    this->close();
}

//生成验证码：随机生成一个验证码，并且显示在界面上
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

//刷新验证码
void loginManager::on_pushButton_2_clicked()
{
    this->generateCaptcha();
}
