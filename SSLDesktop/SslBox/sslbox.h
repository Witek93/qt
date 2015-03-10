#ifndef SSLBOX_H
#define SSLBOX_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QUdpSocket>

#include "sslboxmodel.h"

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
    void on_udpListeningButton_clicked();
    void on_initTcpButton_clicked();
    void on_pairButton_clicked();

    void logOnTextEdit(QString str);
    void setLabel(QString str);

private:
    Ui::SslBox *ui;
    SslBoxModel *model;
};

#endif // SSLBOX_H
