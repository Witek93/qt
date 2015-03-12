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
    if(!serverIsListening())
    {
        if(listen(QHostAddress::Any, 8446)) {
            emit message("SERVER: on");
        } else {
            emit message("SERVER: couldn't turn on");
        }
    }
    else
    {
        emit message("SERVER: already listening");
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
    QFile file_key(":/server.key");
    if(file_key.open(QIODevice::ReadOnly))
    {
        key = file_key.readAll();
        file_key.close();
    }
    else
    {

        qDebug() << "key "  << file_key.errorString();
    }

    QFile file_cert(":/server.cert");
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
    m_sslServer->setProtocol(QSsl::AnyProtocol);
    m_sslServer->addCaCertificate(ssl_cert);
    m_sslServer->setLocalCertificate(ssl_cert);
    m_sslServer->setPrivateKey(ssl_key);
    m_sslServer->startServerEncryption();
}

void SslBoxModel::startServer() {


    m_sslServer = new QSslSocket(this);



}

void SslBoxModel::sslErrors(QList<QSslError> list)
{
    m_sslServer->ignoreSslErrors();
    foreach (QSslError it, list) {
        emit message(it.errorString());
    }
}

void SslBoxModel::stateChanged(QAbstractSocket::SocketState)
{
    emit message(" SSL state: " + QString(m_sslServer->state() ));
    qDebug() << m_sslServer->state();
}

void SslBoxModel::incomingConnection(qintptr handle)
{
    m_sslServer = new QSslSocket(this);
    if(m_sslServer->setSocketDescriptor(handle))
    {
        initCertificate();
        connect(m_sslServer,SIGNAL(sslErrors(QList<QSslError>)),this,SLOT(sslErrors(QList<QSslError>)));
        connect(m_sslServer,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(stateChanged(QAbstractSocket::SocketState)));
        connect(m_sslServer,SIGNAL(encrypted()), this, SLOT(encrypted()));
        connect(m_sslServer,SIGNAL(readyRead()),this,SLOT(sslReadyRead()));
        connect(m_sslServer,SIGNAL(disconnected()),this,SLOT(clientDisconnected()));

        QHostAddress peerAddress = m_sslServer->peerAddress();
        emit label("CONNECTED TO: " + peerAddress.toString());
        emit message("Connected via SSL with: " + peerAddress.toString());
    }
    else
    {
        emit message("Couldn't start SSL connection");
        delete m_sslServer;
        return;
    }

}

//void SslBoxModel::TcpNewConnection() {
//    if(!m_TcpSocket) {
//        m_TcpSocket = m_TcpServer->nextPendingConnection();
//        connect(m_TcpSocket, SIGNAL(readyRead()), this, SLOT(TcpReadyRead()));

//        QHostAddress peerAddress = m_TcpSocket->peerAddress();

//    }
//}

void SslBoxModel::encrypted()
{
    emit message("SERVER : encrypted");
}

void SslBoxModel::sslReadyRead() {
    if(m_sslServer->isEncrypted()){
    QString sslMessage = m_sslServer->readAll().data();
    if("COUNT" == sslMessage.mid(0,5)) {
        emit message("SSL: COUNT=" + sslMessage.mid(6));
        m_buttonPresses = sslMessage.mid(6).toInt();
    }
    else if ("USER" == sslMessage.mid(0,4)) {
        emit message("SSL: USER=" + sslMessage.mid(5));
    }
    else {
        emit message("SSL: Not supported message : " + sslMessage);
    }}
    else
    {
        emit message("SERVER: not encrypted");
        emit message(m_sslServer->errorString());
    }
}


void SslBoxModel::pairingProcess() {
    if(isSslConnected()) {
        if(m_buttonPresses > 0) {
            m_buttonPresses--;
            m_sslServer->write("BUTTON_PRESSED");
            emit message("SSL: There is " + QString::number(m_buttonPresses) + " presses left");
        } else {
            emit message("SSL: You should NOT press that button");
        }
    } else {
        emit message("SSL: Cannot send pairing msg, there is no Tcp connection");
        emit message(m_sslServer->errorString());
    }
}

bool SslBoxModel::isSslConnected() {
    return m_sslServer &&  m_sslServer->isEncrypted();
}

void SslBoxModel::clientDisconnected()
{
    emit message("CLIENT DISCONNECTED");
}
