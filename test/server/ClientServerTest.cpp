#include <gtest/gtest.h>

#include <QtNetwork>

#include <thread>
#include <chrono>
#include <future>

#include <server/TcpServer.h>
#include <client/TcpClient.h>

#include "../MessageOutput.h"

TEST(client_server_test, JoinTest1) {
    qInstallMessageHandler(myMessageOutput);

    char *argv[] = {"program name", "arg1", "arg2", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    QCoreApplication app(argc, argv);

    SimpleChat::TcpServer server("my_password123");
    server.listen(4442, QHostAddress::Any);

    SimpleChat::TcpClient client;
    client.serverAddress_ = QHostAddress::LocalHost;
    client.serverPort_ = 4442;
    client.clientName_ = "Foooo1";
    std::async(std::launch::async, [&client] {
        client.connectToHost();
        client.join();
    });

    EXPECT_EQ(app.exec(), 0);
}
