#include <QCoreApplication>

#include <QtNetwork>
#include <server/TcpChatServer.h>

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);
    auto server = std::make_unique<SimpleChat::TcpChatServer>("abra kadabra");
    server->listen(4441, QHostAddress::AnyIPv4);
    app.exec();

    server.reset();

    return 0;
}
