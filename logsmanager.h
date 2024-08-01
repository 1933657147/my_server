#ifndef LOGSMANAGER_H
#define LOGSMANAGER_H

#include <QWidget>
#include <QFile>
#include <QDateTime>
#include <QMessageBox>
#include <QIcon>
#include <QtMath>
#include <QDebug>


namespace Ui {
class logsmanager;
}

class logsmanager : public QWidget
{
    Q_OBJECT

public:
    explicit logsmanager(QWidget *parent = nullptr);
    ~logsmanager();
    static void writeLogs(QByteArray data);

private slots:
    void on_nextBtn_clicked();

    void on_prevBtn_clicked();

    void on_updateBtn_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_spinBox_valueChanged(int arg1);

    void on_toolButton_clicked();

private:
    Ui::logsmanager *ui;
    void readLogs();
    void showLogs();
    static QFile file;
    int pagesize;
    int totalpage;
    int currpage;
    QStringList logsList;
};

#endif // LOGSMANAGER_H
