#include "client.h"
//---------------------------------------------------------------------------------------
Client::Client(QObject *parent)
    : QObject{parent}
{
    connect(&socket, &QTcpSocket::connected, this, &Client::connected);
    connect(&socket, &QTcpSocket::disconnected, this, &Client::disconnected);
    connect(&socket, &QTcpSocket::stateChanged, this, &Client::stateChanged);
    connect(&socket, &QTcpSocket::errorOccurred, this, &Client::errorOccurred);
    connect(&socket, &QTcpSocket::readyRead, this, &Client::readyRead);

    //ssl
    connect(&socket, &QSslSocket::encrypted, this, &Client::encrypted);
    connect(&socket, &QSslSocket::encryptedBytesWritten, this, &Client::encryptedBytesWritten);
    connect(&socket, &QSslSocket::modeChanged, this, &Client::modeChanged);
    connect(&socket, &QSslSocket::peerVerifyError, this, &Client::peerVerifyError);
    connect(&socket, &QSslSocket::preSharedKeyAuthenticationRequired, this, &Client::preSharedKeyAuthenticationRequired);
    connect(&socket, &QSslSocket::sslErrors, this, &Client::sslErrors);
    /*
    QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::HttpProxy);
    proxy.setHostName("217.199.161.35");
    proxy.setPort(80);

    QNetworkProxy::setApplicationProxy(proxy);
    socket.setProxy(proxy);
    */
}
//---------------------------------------------------------------------------------------
void Client::connectToHost(const QString &address, quint16 port)
{
    if(socket.isOpen()) disconnect();

    qInfo() << "Connecting to: " << address << " on port " << port;
    //socket.connectToHost(address, port);

    //socket.ignoreSslErrors();

    socket.setProtocol(QSsl::AnyProtocol);
    socket.connectToHostEncrypted(address, port);
}
//---------------------------------------------------------------------------------------
void Client::disconnect()
{
    socket.close();
    socket.waitForDisconnected();
}
//---------------------------------------------------------------------------------------
void Client::connected()
{
    qInfo() << "Connected!";
    qInfo() << "Sending";
    //socket.write("HELLO\r\n");

    QByteArray data;
    data.append("GET /get HTTP/1.1\r\n");
    data.append("User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.0; Trident/4.0)\r\n");
    data.append("Host: local\r\n");
    data.append("Connection: Close\r\n");
    data.append("\r\n");
    socket.write(data);

    socket.waitForBytesWritten();

}
//---------------------------------------------------------------------------------------
void Client::disconnected()
{
    qInfo() << "Disconnected";
}
//---------------------------------------------------------------------------------------
void Client::errorOccurred(QAbstractSocket::SocketError socketError)
{
    qInfo() << "Error message: " << socket.errorString();
}
//---------------------------------------------------------------------------------------
void Client::stateChanged(QAbstractSocket::SocketState socketState)
{
    QMetaEnum metaEnam = QMetaEnum::fromType<QAbstractSocket::SocketState>();
    qInfo() << metaEnam.valueToKey(socketState);
}
//---------------------------------------------------------------------------------------
void Client::readyRead()
{
    qInfo() << "Data from: " << sender() << " bytes: " << socket.bytesAvailable();
    qInfo() << "Data: " << socket.readAll();
}
//---------------------------------------------------------------------------------------
void Client::encrypted()
{
    qInfo() << "encrypted";
}
//---------------------------------------------------------------------------------------
void Client::encryptedBytesWritten(qint64 written)
{
    qInfo() << "encryptedBytesWritten:" << written ;
}
//---------------------------------------------------------------------------------------
void Client::modeChanged(QSslSocket::SslMode mode)
{
    qInfo() << "modeChanged:" << mode;
}
//---------------------------------------------------------------------------------------
void Client::peerVerifyError(const QSslError &error)
{
    qInfo() << "peerVerifyError" << error.errorString();
}
//---------------------------------------------------------------------------------------
void Client::preSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *authenticator)
{
    qInfo() << "Preshared key required";
}
//---------------------------------------------------------------------------------------
void Client::sslErrors(const QList<QSslError> &errors)
{
    foreach (QSslError er, errors) {
        qInfo() << "error: " << er.errorString();
    }
}
//---------------------------------------------------------------------------------------
