#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QMetaEnum>
#include <QNetworkProxy>
#include <QSslSocket>
#include <QSslPreSharedKeyAuthenticator>

class Client : public QObject
{
    Q_OBJECT
private:
    //QTcpSocket socket;
    QSslSocket socket;
public:
    explicit Client(QObject *parent = nullptr);

signals:
public slots:
    void connectToHost(const QString &address, quint16 port);
    void disconnect();
private slots:
    void connected();
    void disconnected();
    void errorOccurred(QAbstractSocket::SocketError socketError);
    void stateChanged(QAbstractSocket::SocketState socketState);
    void readyRead();
    //ssl
    void encrypted();
    void encryptedBytesWritten(qint64 written);
    void modeChanged(QSslSocket::SslMode mode);
    void peerVerifyError(const QSslError &error);
    void preSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *authenticator);
    void sslErrors(const QList<QSslError> &errors);
};

#endif // CLIENT_H
