#include <iostream>

#include <QCoreApplication>
#include <QtNetwork>

#include <SimpleChatConfig.h>
#include <server/TcpChatServer.h>

std::tuple<QHostAddress, quint16, std::string> serverParams(const QCoreApplication& app) {
    QCommandLineParser parser;
    parser.setApplicationDescription("Simple chat server");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption ipOption(QStringList() << "i" << "ip-address",
                                QCoreApplication::translate("main", "IP address for server to listen on."),
                                QCoreApplication::translate("main", "ip address"));

    QCommandLineOption portOption(QStringList() << "p" << "port",
                                  QCoreApplication::translate("main", "Port for server to listen on."),
                                  QCoreApplication::translate("main", "port"), "4441");

    QCommandLineOption secretOption(QStringList() << "s" << "secret" << "password",
                                    QCoreApplication::translate("main", "Secret used by users to authorize."),
                                    QCoreApplication::translate("main", "secret"));

    parser.addOption(ipOption);
    parser.addOption(portOption);
    parser.addOption(secretOption);

    parser.process(app);

    QHostAddress hostAddress;
    quint16 port = 0;
    QString secret;

    if(parser.isSet(ipOption))
        hostAddress = QHostAddress(parser.value(ipOption));

    if(parser.isSet(portOption))
        port = parser.value(portOption).toUShort();

    if(parser.isSet(secretOption))
        secret = parser.value(secretOption);
    else {
        fprintf(stderr, "%s\n", qPrintable(QCoreApplication::translate("main", "Error: Must specify a secret.")));
        parser.showHelp(1);
    }

    if(hostAddress.isNull())
        hostAddress = QHostAddress::Any;

    return std::make_tuple(hostAddress, port, secret.toStdString());
}

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    auto localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        // hide debug messages
        // std::cout << "Debug: " << localMsg.constData() << std::endl;
        break;
    case QtWarningMsg:
        std::cout << "Warning: " << localMsg.constData() << std::endl;
        break;
    case QtCriticalMsg:
        std::cout << "Critical: " << localMsg.constData() << " " <<
        context.file << " " <<
        context.line << " " <<
        context.function << std::endl;
        break;
    case QtFatalMsg:
        std::cout << "Fatal: " << localMsg.constData() << " " <<
        context.file << " " <<
        context.line << " " <<
        context.function << std::endl;
        abort();
    }
}

int main(int argc, char* argv[]) {
    qInstallMessageHandler(myMessageOutput);

    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("simple chat server");
    QCoreApplication::setApplicationVersion(QString("%1.%2.%3")
                                                    .arg(SimpleChat_VERSION_MAJOR)
                                                    .arg(SimpleChat_VERSION_MINOR)
                                                    .arg(SimpleChat_VERSION_PATCH));

    QHostAddress address;
    quint16 port;
    std::string secret;
    std::tie(address, port, secret) = serverParams(app);

    SimpleChat::TcpChatServer server(secret, &app);
    server.listen(port, address);
    app.exec();

    return 0;
}
