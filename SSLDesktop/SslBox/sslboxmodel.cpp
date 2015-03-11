#include "sslboxmodel.h"

#include <QFile>
#include <QSsl>
#include <QSslKey>
#include <QSslCertificate>

SslBoxModel::SslBoxModel(QObject *parent) :
    QTcpServer(parent),
    m_buttonPresses(0)
{
    m_udpSocket = new QUdpSocket(this);
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
    return this->isListening();
}

void SslBoxModel::initCertificate()
{
    QByteArray key;
    QByteArray cert;
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
    QSslKey ssl_key(key,QSsl::Rsa);
    QSslCertificate ssl_cert(cert);
    m_sslServer->setProtocol(QSsl::SslV3);
    m_sslServer->addCaCertificate(ssl_cert);
    m_sslServer->setLocalCertificate(ssl_cert);
    m_sslServer->setPrivateKey(ssl_key);
}

void SslBoxModel::startServer() {

    m_sslServer = new QSslSocket(this);
    connect(m_sslServer,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(stateChanged(QAbstractSocket::SocketState)));
    initCertificate();
    if(listen(QHostAddress::Any, 8445)) {
        emit message("SERVER: on");
    } else {
        emit message("SERVER: couldn't turn on");
    }

}

void SslBoxModel::stateChanged(QAbstractSocket::SocketState)
{
    emit message("Tcp state : " + m_sslServer->state() );
}

void SslBoxModel::incomingConnection(qintptr handle)
{
    if(m_sslServer->setSocketDescriptor(handle))
    {
        connect(m_sslServer,SIGNAL(readyRead()), this, SLOT(sslReadyRead()));
        connect(m_sslServer,SIGNAL(disconnected()),this,SLOT(clientDisconnected()));

        QHostAddress peerAddress = m_sslServer->peerAddress();
        emit label("CONNECTED TO: " + peerAddress.toString());
        emit message("Connected via Tcp with: " + peerAddress.toString());
    }
    else
    {
        emit message("Couldn't start Tcp connection");
    }
}

//void SslBoxModel::TcpNewConnection() {
//    if(!m_TcpSocket) {
//        m_TcpSocket = m_TcpServer->nextPendingConnection();
//        connect(m_TcpSocket, SIGNAL(readyRead()), this, SLOT(TcpReadyRead()));

//        QHostAddress peerAddress = m_TcpSocket->peerAddress();

//    }
//}

void SslBoxModel::sslReadyRead() {
    QString sslMessage = m_sslServer->readAll().data();

    if("COUNT" == sslMessage.mid(0,5)) {
        emit message("Tcp: COUNT=" + sslMessage.mid(6));
        m_buttonPresses = sslMessage.mid(6).toInt();
    }
    else if ("USER" == sslMessage.mid(0,4)) {
        emit message("Tcp: USER=" + sslMessage.mid(5));
    }
    else {
        emit message("Tcp: Not supported message : " + sslMessage);
    }
}


void SslBoxModel::pairingProcess() {
    if(isSslConnected()) {
        if(m_buttonPresses > 0) {
            m_buttonPresses--;
            m_sslServer->write("BUTTON_PRESSED");
            emit message("Tcp: There is " + QString::number(m_buttonPresses) + " presses left");
        } else {
            emit message("Tcp: You should NOT press that button");
        }
    } else {
        emit message("Tcp: Cannot send pairing msg, there is no Tcp connection");
    }
}

bool SslBoxModel::isSslConnected() {
    return m_sslServer &&  m_sslServer->isEncrypted();
}

void SslBoxModel::clientDisconnected()
{
    emit message("CLIENT DISCONNECTED");
}
