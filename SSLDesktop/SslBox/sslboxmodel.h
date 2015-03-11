#ifndef SSLBOXMODEL_H
#define SSLBOXMODEL_H

#include <QObject>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QTcpServer>

class SslBoxModel : public QObject
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

private:
//    bool isTcpUnconnected();
    bool isTcpConnected();
    void processPairing();

signals:
    void message(QString msg);
    void label(QString msg);

public slots:
    void tcpNewConnection();
    void tcpReadyRead();
    void broadcastAndStartServer();
    void pairingProcess();

private:
    QUdpSocket *m_udpSocket;
    QTcpSocket *m_tcpSocket;
    QTcpServer *m_tcpServer;
    QHostAddress currentServerAddress;
    int m_buttonPresses;
};

#endif // SSLBOXMODEL_H
