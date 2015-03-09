#include "sslapp.h"
#include "ui_sslapp.h"

#include <QTime>

SslApp::SslApp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SslApp),
    m_buttonPresses(0)
{
    ui->setupUi(this);
    this->setWindowTitle("SSL APP");

    qsrand(QTime::currentTime().msec());

    m_udpSocket = new QUdpSocket(this);

    m_tcpServer = new QTcpServer(this);
    connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(tcpNewConnection()));

    if(!m_tcpServer->listen(QHostAddress::Any, 8445)) {
        logOnTextEdit("Server couldn't start");
    }

    m_tcpSocket = NULL;
}

SslApp::~SslApp()
{
    delete ui;
}

void SslApp::on_startBrodcastButton_clicked()
{
    logOnTextEdit("Broadcasted initial msg");
    QByteArray msg = "INIT";
    m_udpSocket->writeDatagram(msg, msg.size(), QHostAddress::Broadcast, 8445);
}


void SslApp::tcpNewConnection()
{
    if(!m_tcpSocket) {
        m_tcpSocket = m_tcpServer->nextPendingConnection();
        connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));

        QHostAddress peerAddress = m_tcpSocket->peerAddress();
        ui->hostsConnectedLabel->setText("CONNECTED TO: " + peerAddress.toString());
        logOnTextEdit("Connected via TCP with: " + peerAddress.toString());
    }
}

void SslApp::readyRead()
{
    QString str = m_tcpSocket->readAll().data();
    if("BUTTON_PRESSED" == str.mid(0,14)) {
        m_buttonPresses--;
        if(0 == m_buttonPresses) {
            logOnTextEdit("TCP: Devices can be paired now");
        }
        else if (0 < m_buttonPresses) {
            logOnTextEdit("TCP: Button has been pressed");
        }
        else {
            logOnTextEdit("TCP: You should not get here...");
        }
    }
    else {
        logOnTextEdit("Not supported msg: " + str);
    }
}


void SslApp::logOnTextEdit(QString string)
{
    ui->logTextEdit->textCursor().insertText(string + "\n");
}

void SslApp::on_closeConnectionButton_clicked()
{
    if(m_tcpSocket) {
        m_tcpSocket->close();
        ui->hostsConnectedLabel->setText("NO HOST CONNECTED");
        logOnTextEdit("Connection closed");
        m_tcpSocket = NULL;
    }
}

void SslApp::on_pressCountButton_clicked()
{
    if(m_tcpSocket) {
        m_buttonPresses = qrand() % 4 + 3;
        QString count = "COUNT=" + QString::number(m_buttonPresses);
        m_tcpSocket->write(count.toUtf8(), count.size());
        m_tcpSocket->flush();
        m_tcpSocket->waitForBytesWritten(1000);
    }
}
