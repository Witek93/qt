#include "sslappmodel.h"

SslAppModel::SslAppModel(QObject *parent) :
    QObject(parent)
{
    m_udpSocket = new QUdpSocket(this);
    connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(udpReadyRead()));

    m_tcpSocket = new QTcpSocket(this);
    connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(m_tcpSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(m_tcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));

//    m_buttonPresses = 0;
}

SslAppModel::~SslAppModel()
{
}


void SslAppModel::udpReadyRead() {
    while(m_udpSocket->hasPendingDatagrams()) {
        QByteArray data;
        data.resize(m_udpSocket->pendingDatagramSize());

        m_udpSocket->readDatagram(data.data(), data.size(), &currentServerAddress);

        emit message("UDP: Got msg from " + currentServerAddress.toString());
        emit label("SERVER: " + currentServerAddress.toString());
        initConnection();
    }
}

void SslAppModel::listen() {
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

bool SslAppModel::isUdpBound() {
    return m_udpSocket && m_udpSocket->state() == QUdpSocket::BoundState;
}

bool SslAppModel::isUdpUnconnected() {
    return m_udpSocket && m_udpSocket->state() == QUdpSocket::UnconnectedState;
}

void SslAppModel::readyRead() {
    QString str = m_tcpSocket->readAll().data();
    if("BUTTON_PRESSED" == str.mid(0,14)) {
        processButtonPress();
    }
    else {
        emit message("Not supported msg: " + str);
    }
}

void SslAppModel::processButtonPress() {
    m_buttonPresses--;
    if(0 == m_buttonPresses) {
        emit message("TCP: Devices can be paired now");
    }
    else if (0 < m_buttonPresses) {
        emit message("TCP: Button has been pressed");
    }
}

void SslAppModel::connected() {
    emit message("TCP: Connected");
}

void SslAppModel::disconnected() {
    emit message("TCP: Disconnected");
}


void SslAppModel::sendButtonPressCount() {
    if(m_tcpSocket && m_tcpSocket->isWritable()) {
        m_buttonPresses = qrand() % 4 + 3;
        QString count = "COUNT=" + QString::number(m_buttonPresses);
        m_tcpSocket->write(count.toUtf8(), count.size());
    }
}

void SslAppModel::initConnection() {
    if(!currentServerAddress.isNull()) {
        if(isTcpUnconnected()) {
            m_tcpSocket->connectToHost(currentServerAddress, 8445);
        }
    } else {
        emit message("TCP: There is no server to connect to!");
    }
}

bool SslAppModel::isTcpUnconnected() {
    return m_tcpSocket && m_tcpSocket->state() == QTcpSocket::UnconnectedState;
}
