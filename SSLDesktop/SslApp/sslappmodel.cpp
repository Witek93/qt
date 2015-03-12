#include "sslappmodel.h"
#include <QFile>
#include <QSsl>
#include <QSslKey>
#include <QSslCertificate>
SslAppModel::SslAppModel(QObject *parent) :
    QObject(parent)
{
    m_udpSocket = new QUdpSocket(this);
    connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(udpReadyRead()));

    m_sslSocket = new QSslSocket(this);
//    initCertificate();
    connect(m_sslSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(m_sslSocket, SIGNAL(encrypted()), this, SLOT(encrypted()));
    connect(m_sslSocket, SIGNAL(connected()), this,SLOT(connected()));
    connect(m_sslSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(m_sslSocket, SIGNAL(sslErrors(QList<QSslError>)),this,SLOT(sslErrors(QList<QSslError>)));
    m_buttonPresses = 0;
}

SslAppModel::~SslAppModel()
{
}

void SslAppModel::initCertificate()
{
    qDebug() << "1";
    QByteArray key;
    qDebug() << "2";
    QByteArray cert;
    qDebug() << "3";
    QFile file_key(":/api.xyz.com.key");
    if(file_key.open(QIODevice::ReadOnly))
    {
        key = file_key.readAll();
        file_key.close();
    }
    else
    {

        qDebug() << "key "  << file_key.errorString();
    }

    QFile file_cert(":/api.xyz.com.crt");
    if(file_cert.open(QIODevice::ReadOnly))
    {
        cert = file_cert.readAll();
        file_cert.close();
    }
    else
    {
        qDebug() << "csr " << file_cert.errorString();
    }
    qDebug() << "3";
    QSslKey ssl_key(key,QSsl::Rsa);
    qDebug() << "4";
    QSslCertificate ssl_cert(cert);
    qDebug() << "5";
    m_sslSocket->setProtocol(QSsl::SslV3);
    qDebug() << "6";
    m_sslSocket->addCaCertificate(ssl_cert);
    qDebug() << "7";
    m_sslSocket->setLocalCertificate(ssl_cert);
    qDebug() << "8";
    m_sslSocket->setPrivateKey(ssl_key);
    qDebug() << "9";
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
        emit message("SSL: Devices can be paired now");
    }
    else if (0 < m_buttonPresses) {
        emit message("SSL: Button has been pressed");
    }
}

void SslAppModel::encrypted()
{
    emit message("SSL : Encrypted");
}

void SslAppModel::connected() {
    emit message("SSL: Connected");
    emit message(isSslUnconnected() ? "true ":"false");
}

void SslAppModel::disconnected() {
    emit message("SSL: Disconnected");
}


void SslAppModel::sendButtonPressCount() {
    if(m_sslSocket && m_sslSocket->isEncrypted()) {
        m_buttonPresses = qrand() % 4 + 3;
        QString count = "COUNT=" + QString::number(m_buttonPresses);
        m_sslSocket->write(count.toUtf8(), count.size());
    }
    else
    {
    }
}

void SslAppModel::initConnection() {
    if(!currentServerAddress.isNull()) {
        if(isSslUnconnected()) {
            m_sslSocket->connectToHostEncrypted(currentServerAddress.toString(), 8446);
        }
    } else {
        emit message("SSL: There is no server to connect to!");
    }
}

bool SslAppModel::isSslUnconnected() {
    return m_sslSocket && m_sslSocket->state() == QSslSocket::UnconnectedState;
}

void SslAppModel::sslErrors(QList<QSslError> list)
{
    m_sslSocket->ignoreSslErrors();
    foreach (QSslError it, list) {
        emit message(it.errorString());
    }
}
