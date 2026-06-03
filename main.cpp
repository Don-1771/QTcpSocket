#include <QCoreApplication>
#include "client.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Client client;
    client.connectToHost("www.httpbin.org", 443); //443 — это сетевой порт для безопасных (зашифрованных) соединений по протоколу HTTPS

    return a.exec();
}
