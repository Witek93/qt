#include "sslboxmodel.h"

SslBoxModel::SslBoxModel(QObject *parent) :
    QObject(parent),
    m_buttonPresses(0)
{
    m_udpSocket = new QUdpSocket(this);
    m_tcpServer = NULL;
    m_tcpSocket = NULL;
}

SslBoxModel::~SslBoxModel() {}


void SslBoxModel::broadcastAndStartServer() {
    broadcast("INIT");
    emit message("UDP: Broadcast");

    if(!serverIsListening()) {
        startServer();
    } else {
        emit message("SERVER: already turned on");
    }
}

void SslBoxModel::broadcast(QByteArray msg) {
    if(m_udpSocket) {
        m_udpSocket->writeDatagram(msg, msg.size(), QHostAddress::Broadcast, 8445);
    }
}

bool SslBoxModel::serverIsListening() {
    return m_tcpServer && m_tcpServer->isListening();
}

void SslBoxModel::startServer() {
    m_tcpServer = new QTcpServer(this);
    if(m_tcpServer->listen(QHostAddress::Any, 8445)) {
        emit message("SERVER: on");
    } else {
        emit message("SERVER: couldn't turn on");
    }
    connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(tcpNewConnection()));
}

void SslBoxModel::tcpNewConnection() {
    if(!m_tcpSocket) {
        m_tcpSocket = m_tcpServer->nextPendingConnection();
        connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(tcpReadyRead()));

        QHostAddress peerAddress = m_tcpSocket->peerAddress();
        emit label("CONNECTED TO: " + peerAddress.toString());
        emit message("Connected via TCP with: " + peerAddress.toString());
    }
}

void SslBoxModel::tcpReadyRead() {
    QString tcpMessage = m_tcpSocket->readAll().data();

    if("COUNT" == tcpMessage.mid(0,5)) {
        emit message("TCP: COUNT=" + tcpMessage.mid(6));
        m_buttonPresses = tcpMessage.mid(6).toInt();
    }
    else if ("USER" == tcpMessage.mid(0,4)) {
        emit message("TCP: USER=" + tcpMessage.mid(5));
    }
    else {
        emit message("TCP: Not supported message");
    }
}


void SslBoxModel::pairingProcess() {
    if(isTcpConnected()) {
        if(m_buttonPresses > 0) {
            m_buttonPresses--;
            m_tcpSocket->write("BUTTON_PRESSED");
            emit message("TCP: There is " + QString::number(m_buttonPresses) + " presses left");
        } else {
            emit message("TCP: You should NOT press that button");
        }
    } else {
        emit message("TCP: Cannot send pairing msg, there is no TCP connection");
    }
}

bool SslBoxModel::isTcpConnected() {
    return m_tcpSocket && m_tcpSocket->state() == QTcpSocket::ConnectedState;
}
