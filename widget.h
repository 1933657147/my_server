#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include<QPainter>
#include "homepage.h"
#include "mytcpclient.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    homepage *homep;

    void resizeEvent(QResizeEvent *event);


public slots:
    void setPage(int m);

private slots:
    void on_tabWidget_currentChanged(int index);

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
