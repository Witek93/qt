#include "sslappmodel.h"

SslAppModel::SslAppModel(QObject *parent) :
    QObject(parent)
{
    m_udpSocket = new QUdpSocket(this);

    m_tcpServer = new QTcpServer(this);
    connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(tcpNewConnection()));

    m_tcpSocket = NULL;
    m_buttonPresses = 0;
}

SslAppModel::~SslAppModel()
{
}


void SslAppModel::startBroadcast(QByteArray msg) {
    m_udpSocket->writeDatagram(msg, msg.size(), QHostAddress::Broadcast, 8445);
}

void SslAppModel::sendButtonPressCount() {
    if(m_tcpSocket) {
        m_buttonPresses = qrand() % 4 + 3;
        QString count = "COUNT=" + QString::number(m_buttonPresses);
        m_tcpSocket->write(count.toUtf8(), count.size());
        m_tcpSocket->flush();
        m_tcpSocket->waitForBytesWritten(1000);
    }
}


void SslAppModel::closeConnection() {
    if(m_tcpSocket) {
        m_tcpSocket->close();
        m_tcpSocket = NULL;
    }
}


bool SslAppModel::serverIsListening() {
    return m_tcpServer->listen(QHostAddress::Any, 8445);
}

bool SslAppModel::connectionIsClosed() {
    return !m_tcpSocket || !m_tcpSocket->isOpen();
}

void SslAppModel::tcpNewConnection() {
    if(!m_tcpSocket) {
        m_tcpSocket = m_tcpServer->nextPendingConnection();
        connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));

        QHostAddress peerAddress = m_tcpSocket->peerAddress();
        emit label("CONNECTED TO" + peerAddress.toString());
        emit message("Connected via TCP with: " + peerAddress.toString());
    }
}

void SslAppModel::readyRead() {
    QString str = m_tcpSocket->readAll().data();
    if("BUTTON_PRESSED" == str.mid(0,14)) {
        m_buttonPresses--;
        if(0 == m_buttonPresses) {
            emit message("TCP: Devices can be paired now");
        }
        else if (0 < m_buttonPresses) {
            emit message("TCP: Button has been pressed");
        }
        else {
            emit message("TCP: You should not get here...");
        }
    }
    else {
        emit message("Not supported msg: " + str);
    }
}
