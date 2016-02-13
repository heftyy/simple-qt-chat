#include <QCoreApplication>

#include <QtNetwork>
#include <server/TcpChatServer.h>

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);
    SimpleChat::TcpChatServer server("abra_kadabra", &app);
    server.listen(4441, QHostAddress::AnyIPv4);
    app.exec();   

    return 0;
}
