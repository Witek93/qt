#ifndef SSLBOXMODEL_H
#define SSLBOXMODEL_H

#include <QObject>
#include <QUdpSocket>
#include <QTcpSocket>

class SslBoxModel : public QObject
{
    Q_OBJECT
public:
    explicit SslBoxModel(QObject *parent = 0);
    ~SslBoxModel();

    void listen();
    void initConnection();
    void sendPairingMessage();
    bool isConnected();

private:
    bool isTcpUnconnected();
    bool isTcpConnected();
    void processPairingPress();
    bool isUdpBound();
    bool isUdpUnconnected();

signals:
    void message(QString msg);
    void label(QString msg);

public slots:
    void udpReadyRead();

    void connected();
    void disconnected();
    void readyRead();

private:
    QUdpSocket *m_udpSocket;
    QTcpSocket *m_tcpSocket;
    QHostAddress currentServerAddress;
    int m_buttonPresses;
};

#endif // SSLBOXMODEL_H
