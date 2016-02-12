#include <QCoreApplication>

#include <server/ChatServer.h>

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);
    SimpleChat::ChatServer server("abra kadabra");
    server.listen(4441, QHostAddress::AnyIPv4);
    app.exec();

    return 0;
}
