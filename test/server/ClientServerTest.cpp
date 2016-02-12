#include <gtest/gtest.h>

#include <QtNetwork>

#include <thread>
#include <chrono>
#include <future>

#include <server/ChatServer.h>
#include <client/ChatClient.h>

#include "../MessageOutput.h"

TEST(client_server_test, JoinTest1) {
    qInstallMessageHandler(myMessageOutput);

    char *argv[] = {"program name", "arg1", "arg2", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    QCoreApplication app(argc, argv);

    SimpleChat::ChatServer server("my_password123");
    server.listen(4442, QHostAddress::Any);

    EXPECT_EQ(app.exec(), 0);
}
