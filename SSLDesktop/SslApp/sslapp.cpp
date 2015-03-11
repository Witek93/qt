#include "sslapp.h"
#include "ui_sslapp.h"

SslApp::SslApp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SslApp)
{
    ui->setupUi(this);
    setWindowTitle("Tcp APP");

    model = new SslAppModel(this);
    connect(model, SIGNAL(message(QString)), this, SLOT(logOnTextEdit(QString)));
    connect(model, SIGNAL(label(QString)), this, SLOT(setLabel(QString)));
}

SslApp::~SslApp() {
    delete ui;
}

void SslApp::on_listenButton_clicked() {
    model->listen();
}

void SslApp::on_sendPressCountButton_clicked() {
    model->sendButtonPressCount();
}

void SslApp::logOnTextEdit(QString str) {
    QTextCursor cursor = ui->logTextEdit->textCursor();
    cursor.insertText(str + "\n");
    cursor.movePosition(QTextCursor::End);
    ui->logTextEdit->setTextCursor(cursor);
}

void SslApp::setLabel(QString str) {
    ui->hostsConnectedLabel->setText(str);
}

