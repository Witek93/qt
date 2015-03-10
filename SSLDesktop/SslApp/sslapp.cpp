#include "sslapp.h"
#include "ui_sslapp.h"

SslApp::SslApp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SslApp)
{
    model = new SslAppModel(this);

    ui->setupUi(this);
    setWindowTitle("SSL APP");

    if(!model->serverIsListening()) {
        logOnTextEdit("Server couldn't start");
    }

    connect(model, SIGNAL(message(QString)), this, SLOT(logOnTextEdit(QString)));
    connect(model, SIGNAL(label(QString)), this, SLOT(setLabel(QString)));
}


SslApp::~SslApp()
{
    delete ui;
}


void SslApp::on_startBrodcastButton_clicked()
{
    logOnTextEdit("Broadcasted initial msg");
    model->startBroadcast("INIT");
}


void SslApp::on_closeConnectionButton_clicked()
{
    model->closeConnection();
    if(model->connectionIsClosed()) {
        logOnTextEdit("Connection is closed");
        setLabel("NO HOST CONNECTED");
    } else {
        logOnTextEdit("Connection cannot be closed");
    }
}


void SslApp::on_pressCountButton_clicked()
{
    model->sendButtonPressCount();
}


void SslApp::logOnTextEdit(QString str)
{
    ui->logTextEdit->textCursor().insertText(str + "\n");
}


void SslApp::setLabel(QString str)
{
    ui->hostsConnectedLabel->setText(str);
}


