#include "sslbox.h"
#include "ui_sslbox.h"

SslBox::SslBox(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SslBox),
    m_buttonPresses(0)
{
    ui->setupUi(this);
    this->setWindowTitle("SSL BOX");

    m_tcpSocket = new QTcpSocket(this);
    connect(m_tcpSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(m_tcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    m_udpSocket = new QUdpSocket(this);
    connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(udpReadyRead()));
}


SslBox::~SslBox()
{
    delete ui;
}


void SslBox::connected()
{
    logOnTextEdit("TCP: Connected");
}


void SslBox::disconnected()
{
    logOnTextEdit("TCP: Disconnected");
}


void SslBox::readyRead() {
    QString str = m_tcpSocket->readAll().data();

    if("COUNT" == str.mid(0,5)) {
        logOnTextEdit("TCP: COUNT=" + str.mid(6));
        m_buttonPresses = str.mid(6).toInt();
    }
    else if ("USER" == str.mid(0,4)) {
        logOnTextEdit("TCP: USER=" + str.mid(5));
    }
    else {
        logOnTextEdit("TCP: Not supported message");
    }
}


void SslBox::on_udpListeningButton_clicked()
{
    if(m_udpSocket->state() == QUdpSocket::UnconnectedState) {
        logOnTextEdit("Listening for UDP connection...");
        m_udpSocket->bind(8445, QUdpSocket::ShareAddress);
    }
}


void SslBox::udpReadyRead()
{
    while(m_udpSocket->hasPendingDatagrams()) {
        QByteArray data;
        data.resize(m_udpSocket->pendingDatagramSize());
        m_udpSocket->readDatagram(data.data(), data.size(), &currentServerAddress);
        ui->hostLabel->setText("Server address is:" + currentServerAddress.toString());
        logOnTextEdit("UDP: Got msg");
    }
}


void SslBox::on_initTcpButton_clicked()
{
    if(!currentServerAddress.isNull()) {
        logOnTextEdit("Trying to connect to " + currentServerAddress.toString());
        if(m_tcpSocket->state() == QTcpSocket::UnconnectedState) {
            m_tcpSocket->connectToHost(currentServerAddress, 8445);
        }
    } else {
        logOnTextEdit("There is no server to connect to!");
    }
}


void SslBox::on_pairButton_clicked()
{
    if(m_tcpSocket && m_tcpSocket->isOpen()) {
        if(m_buttonPresses > 0) {
            m_buttonPresses--;
            m_tcpSocket->write("BUTTON_PRESSED");
            logOnTextEdit("There is " + QString::number(m_buttonPresses) + " left");
        } else {
            logOnTextEdit("TCP: You should stop pressing that button");
        }
    } else {
        logOnTextEdit("TCP: There is no connection!");
    }
}


void SslBox::logOnTextEdit(QString string)
{
    QTextCursor cursor = ui->logTextEdit->textCursor();
    cursor.insertText(string + "\n");
    cursor.movePosition(QTextCursor::End);
    ui->logTextEdit->setTextCursor(cursor);
}

