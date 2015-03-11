#ifndef TcpAPPMODEL_H
#define TcpAPPMODEL_H

#include <QObject>
#include <QUdpSocket>
//#include <QTcpSocket>
#include <QSslSocket>
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
    bool isSslUnconnected();

signals:
    void message(QString msg);
    void label(QString msg);

public slots:
    void udpReadyRead();
    void readyRead();
    void connected();
    void disconnected();
    void sslErrors(QList<QSslError> list);
private:
    QUdpSocket *m_udpSocket;
    QHostAddress currentServerAddress;
    QSslSocket*m_sslSocket;
    int m_buttonPresses;
};

#endif // TcpAPPMODEL_H
