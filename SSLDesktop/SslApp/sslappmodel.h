#ifndef SSLAPPMODEL_H
#define SSLAPPMODEL_H

#include <QObject>
#include <QUdpSocket>
#include <QTcpServer>
#include <QTcpSocket>

class SslAppModel : public QObject
{
    Q_OBJECT
public:
    explicit SslAppModel(QObject *parent = 0);
    ~SslAppModel();

    void startBroadcast(QByteArray msg);
    void sendButtonPressCount();
    void closeConnection();
    bool serverIsListening();
    bool connectionIsClosed();

signals:
    void message(QString msg);
    void label(QString msg);

public slots:
    void tcpNewConnection();
    void readyRead();

private:
    QUdpSocket *m_udpSocket;
    QTcpServer *m_tcpServer;
    QTcpSocket *m_tcpSocket;
    int m_buttonPresses;
};

#endif // SSLAPPMODEL_H
