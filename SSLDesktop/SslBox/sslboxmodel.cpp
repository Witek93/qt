#include "sslboxmodel.h"

SslBoxModel::SslBoxModel(QObject *parent) :
    QObject(parent),
    m_buttonPresses(0)
{
    m_udpSocket = new QUdpSocket(this);
    connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(udpReadyRead()));

    m_tcpSocket = new QTcpSocket(this);
    connect(m_tcpSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(m_tcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}


SslBoxModel::~SslBoxModel() {}


void SslBoxModel::udpReadyRead() {
    while(m_udpSocket->hasPendingDatagrams()) {
        QByteArray data;
        data.resize(m_udpSocket->pendingDatagramSize());

        m_udpSocket->readDatagram(data.data(), data.size(), &currentServerAddress);

        emit message("UDP: Got msg from " + currentServerAddress.toString());
        emit label("SERVER: " + currentServerAddress.toString());
    }
}


void SslBoxModel::connected() {
    emit message("TCP: Connected");
}


void SslBoxModel::disconnected() {
    emit message("TCP: Disconnected");
}


void SslBoxModel::readyRead() {
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



void SslBoxModel::listen() {
    if(isUdpUnconnected()) {
        emit message("UDP: Binding connection...");
        m_udpSocket->bind(8445, QUdpSocket::ShareAddress);
    }
    else if(isUdpBound()) {
        emit message("UDP: Already bound");
    } else {
        emit message("UDP: Connection cannot be bound");
    }
}

bool SslBoxModel::isUdpBound() {
    return m_udpSocket && m_udpSocket->state() == QUdpSocket::BoundState;
}

bool SslBoxModel::isUdpUnconnected() {
    return m_udpSocket && m_udpSocket->state() == QUdpSocket::UnconnectedState;
}


void SslBoxModel::initConnection() {
    if(!currentServerAddress.isNull()) {
        if(isTcpUnconnected()) {
            m_tcpSocket->connectToHost(currentServerAddress, 8445);
        }
    } else {
        emit message("TCP: There is no server to connect to!");
    }
}

bool SslBoxModel::isTcpUnconnected() {
    return m_tcpSocket && m_tcpSocket->state() == QTcpSocket::UnconnectedState;
}


void SslBoxModel::sendPairingMessage() {
    if(isTcpConnected()) {
        processPairingPress();
    } else {
        emit message("TCP: Cannot send pairing msg, there is no TCP connection");
    }
}

bool SslBoxModel::isTcpConnected() {
    return m_tcpSocket && m_tcpSocket->state() == QTcpSocket::ConnectedState;
}

void SslBoxModel::processPairingPress() {
    if(m_buttonPresses > 0) {
        m_buttonPresses--;
        m_tcpSocket->write("BUTTON_PRESSED");
        emit message("TCP: There is " + QString::number(m_buttonPresses) + " presses left");
    } else {
        emit message("TCP: You should NOT press that button");
    }
}
