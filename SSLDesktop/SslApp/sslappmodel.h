#ifndef SSLAPPMODEL_H
#define SSLAPPMODEL_H

#include <QObject>
#include <QUdpSocket>
#include <QTcpSocket>

class SslAppModel : public QObject
{
    Q_OBJECT
public:
    explicit SslAppModel(QObject *parent = 0);
    ~SslAppModel();

    void listen();
    void processButtonPress();
    void sendButtonPressCount();
    void initConnection();
//    void closeConnection();
//    bool connectionIsClosed();

private:
    bool isUdpBound();
    bool isUdpUnconnected();
    bool isTcpUnconnected();

signals:
    void message(QString msg);
    void label(QString msg);

public slots:
    void udpReadyRead();
    void readyRead();
    void connected();
    void disconnected();

private:
    QUdpSocket *m_udpSocket;
    QHostAddress currentServerAddress;
    QTcpSocket *m_tcpSocket;
    int m_buttonPresses;
};

#endif // SSLAPPMODEL_H
