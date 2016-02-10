#include <gtest/gtest.h>

#include <thread>
#include <chrono>

#include <server/TcpServer.h>
#include <client/TcpClient.h>

TEST(client_server_test, JoinTest) {
    int argc;
    char* argv[] = { "blam.exe" };
    QCoreApplication app(argc, argv);

    SimpleChat::TcpServer server("my_password123");
    server.listen(4441, QHostAddress(QString("192.168.0.100")));

    SimpleChat::TcpClient client;
    client.serverAddress_ = QHostAddress(QString("192.168.0.100"));
    client.serverPort_ = 4441;
    client.clientName_ = "Foooo1";
    client.connect();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    client.join();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    client.sendMessage("This is a message to from MEEE", "notarget");

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    EXPECT_EQ(app.exec(), 0);
}
