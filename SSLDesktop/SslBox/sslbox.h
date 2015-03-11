#ifndef TcpBOX_H
#define TcpBOX_H

#include "sslboxmodel.h"

#include <QMainWindow>
#include <QTimer>

namespace Ui {
class SslBox;
}

class SslBox : public QMainWindow
{
    Q_OBJECT

public:
    explicit SslBox(QWidget *parent = 0);
    ~SslBox();

private slots:
    void on_topButton_pressed();
    void on_topButton_released();

    void logOnTextEdit(QString str);
    void setLabel(QString str);

private:
    Ui::SslBox *ui;
    SslBoxModel *model;
    QTimer *timer;

};

#endif // TcpBOX_H
