#ifndef TcpAPP_H
#define TcpAPP_H

#include "sslappmodel.h"

#include <QMainWindow>

namespace Ui {
class SslApp;
}

class SslApp : public QMainWindow
{
    Q_OBJECT

public:
    explicit SslApp(QWidget *parent = 0);
    ~SslApp();

private slots:
    void on_listenButton_clicked();
    void on_sendPressCountButton_clicked();

    void logOnTextEdit(QString str);
    void setLabel(QString str);

private:
    Ui::SslApp *ui;
    SslAppModel * model;

};

#endif // TcpAPP_H
