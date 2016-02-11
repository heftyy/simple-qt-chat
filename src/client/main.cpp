#include <QApplication>
#include <client/TcpClient.h>
#include <thread>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SimpleChat::TcpClient client;
    client.serverAddress_ = QHostAddress::LocalHost;
    client.serverPort_ = 4441;
    client.clientName_ = "Foooo1";
    client.connectToHost();

    client.join();    
    return app.exec();
}
