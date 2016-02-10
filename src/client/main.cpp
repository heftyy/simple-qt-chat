#include <QApplication>
#include <client/TcpClient.h>
#include <thread>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SimpleChat::TcpClient client;
    client.serverAddress_ = QHostAddress(QString("192.168.0.100"));
    client.serverPort_ = 4441;
    client.clientName_ = "Foooo1";
    client.connect();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    client.join();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    client.sendMessage("aaaaaaaa", "ffff");
    return app.exec();
}
