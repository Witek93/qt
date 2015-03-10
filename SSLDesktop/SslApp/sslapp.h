#ifndef SSLAPP_H
#define SSLAPP_H

#include <QMainWindow>

#include "sslappmodel.h"

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
    void logOnTextEdit(QString str);
    void setLabel(QString str);

    void on_startBrodcastButton_clicked();
    void on_closeConnectionButton_clicked();
    void on_pressCountButton_clicked();

private:
    Ui::SslApp *ui;
    SslAppModel * model;

};

#endif // SSLAPP_H
