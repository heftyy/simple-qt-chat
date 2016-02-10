#include <QCoreApplication>

#include <server/TcpServer.h>

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);
    SimpleChat::TcpServer server("abra kadabra");
    server.listen(4441);
    return app.exec();
}
