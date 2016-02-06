#include <QCoreApplication>
#include <QtCore>

#include "tcp/TcpServer.h"

#include "User.pb.h"

int main(int argc, char* argv[]) {

	SimpleChat::User u;

    QCoreApplication app(argc, argv);
    SimpleChat::TcpServer server;
    server.listen(4441);
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    return app.exec();
}
