#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>

namespace Ui {
class homepage;
}

class homepage : public QWidget
{
    Q_OBJECT

public:
    explicit homepage(QWidget *parent = nullptr);
    ~homepage();

    void paintEvent(QPaintEvent *event);
//    homepage h;
signals:
    void firstSignal(int n);

private slots:
    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

    void on_toolButton_4_clicked();

    void on_toolButton_3_clicked();

    void on_pushButton_clicked();

private:
    Ui::homepage *ui;
};

#endif // HOMEPAGE_H
