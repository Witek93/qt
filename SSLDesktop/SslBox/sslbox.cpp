#include "sslbox.h"
#include "ui_sslbox.h"

SslBox::SslBox(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SslBox)
{
    ui->setupUi(this);
    this->setWindowTitle("SSL BOX");

    model = new SslBoxModel(this);
    connect(model, SIGNAL(message(QString)), this, SLOT(logOnTextEdit(QString)));
    connect(model, SIGNAL(label(QString)), this, SLOT(setLabel(QString)));
}


SslBox::~SslBox()
{
    delete ui;
}


void SslBox::on_udpListeningButton_clicked()
{
    model->listen();
}


void SslBox::on_initTcpButton_clicked()
{
    model->initConnection();
}


void SslBox::on_pairButton_clicked()
{
    model->sendPairingMessage();
}


void SslBox::logOnTextEdit(QString str)
{
    QTextCursor cursor = ui->logTextEdit->textCursor();
    cursor.insertText(str + "\n");
    cursor.movePosition(QTextCursor::End);
    ui->logTextEdit->setTextCursor(cursor);
}


void SslBox::setLabel(QString str)
{
    ui->hostLabel->setText(str);
}

