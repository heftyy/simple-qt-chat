#include <QApplication>
#include "tcp/TcpClient.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SimpleChat::TcpClient client;
    client.serverAddress_ = QHostAddress(QString("192.168.16.71"));
    client.serverPort_ = 4441;
    client.clientName_ = "Foooo1";
    client.connect();
    client.join();

    client.sendMessage("aaaaaaaa", "ffff");
    return app.exec();
}