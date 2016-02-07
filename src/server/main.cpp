#include <QCoreApplication>
#include <QtCore>

#include "tcp/TcpServer.h"

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);
    SimpleChat::TcpServer server;
    server.listen(4441);
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    return app.exec();
}
