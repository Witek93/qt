#ifndef TcpBOXMODEL_H
#define TcpBOXMODEL_H

#include <QObject>
#include <QUdpSocket>
//#include <QTcpSocket>
#include <QTcpServer>
#include <QSslSocket>
class SslBoxModel : public QTcpServer
{
    Q_OBJECT
public:
    explicit SslBoxModel(QObject *parent = 0);
    ~SslBoxModel();

    void broadcast(QByteArray msg);
    void startServer();
    bool serverIsListening();

//    void initConnection();
//    bool isConnected();

    void initCertificate();
private:
//    bool isTcpUnconnected();
    bool isSslConnected();
    void processPairing();

signals:
    void message(QString msg);
    void label(QString msg);

protected:
    void incomingConnection(qintptr handle);

public slots:
//    void TcpNewConnection();
    void sslReadyRead();
    void broadcastAndStartServer();
    void pairingProcess();
    void clientDisconnected();
    void stateChanged(QAbstractSocket::SocketState);
private:
    QUdpSocket *m_udpSocket;
//    QTcpSocket *m_TcpSocket;
//    QTcpServer *m_tcpServer;
    QSslSocket *m_sslServer;
    QHostAddress currentServerAddress;
    int m_buttonPresses;
};

#endif // TcpBOXMODEL_H
