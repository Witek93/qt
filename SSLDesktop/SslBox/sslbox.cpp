#include "sslbox.h"
#include "ui_sslbox.h"

SslBox::SslBox(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SslBox)
{
    ui->setupUi(this);
    this->setWindowTitle("Tcp BOX");

    timer = new QTimer(this);
    timer->setSingleShot(true);

    model = new SslBoxModel(this);
    connect(model, SIGNAL(message(QString)), this, SLOT(logOnTextEdit(QString)));
    connect(model, SIGNAL(label(QString)), this, SLOT(setLabel(QString)));
}

SslBox::~SslBox() {
    delete ui;
}

void SslBox::on_topButton_pressed() {
    timer->start(1000);
}

void SslBox::on_topButton_released() {
    if(timer->remainingTime() == -1) {
        model->broadcastAndStartServer();
    } else {
        model->pairingProcess();
    }
}

void SslBox::logOnTextEdit(QString str) {
    QTextCursor cursor = ui->logTextEdit->textCursor();
    cursor.insertText(str + "\n");
    cursor.movePosition(QTextCursor::End);
    ui->logTextEdit->setTextCursor(cursor);
}

void SslBox::setLabel(QString str) {
    ui->hostLabel->setText(str);
}

