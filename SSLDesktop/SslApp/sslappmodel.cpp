#include "sslappmodel.h"

SslAppModel::SslAppModel(QObject *parent) :
    QObject(parent)
{
    m_udpSocket = new QUdpSocket(this);
    connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(udpReadyRead()));

    m_sslSocket = new QSslSocket(this);
    connect(m_sslSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(m_sslSocket, SIGNAL(encrypted()), this, SLOT(connected()));
    connect(m_sslSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(m_sslSocket, SIGNAL(sslErrors(QList<QSslError>)),this,SLOT(sslErrors(QList<QSslError>)));
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
    QString str = m_sslSocket->readAll().data();
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
        emit message("Tcp: Devices can be paired now");
    }
    else if (0 < m_buttonPresses) {
        emit message("Tcp: Button has been pressed");
    }
}

void SslAppModel::connected() {
    emit message("Tcp: Connected");
}

void SslAppModel::disconnected() {
    emit message("Tcp: Disconnected");
}


void SslAppModel::sendButtonPressCount() {
    if(m_sslSocket && m_sslSocket->isWritable()) {
        m_buttonPresses = qrand() % 4 + 3;
        QString count = "COUNT=" + QString::number(m_buttonPresses);
        m_sslSocket->write(count.toUtf8(), count.size());
    }
}

void SslAppModel::initConnection() {
    if(!currentServerAddress.isNull()) {
        if(isSslUnconnected()) {
            m_sslSocket->connectToHostEncrypted(currentServerAddress.toString(), 8445);
        }
    } else {
        emit message("Tcp: There is no server to connect to!");
    }
}

bool SslAppModel::isSslUnconnected() {
    return m_sslSocket && m_sslSocket->state() == QSslSocket::UnconnectedState;
}

void SslAppModel::sslErrors(QList<QSslError> list)
{
    foreach (QSslError it, list) {
        emit message(it.errorString());
    }
}
