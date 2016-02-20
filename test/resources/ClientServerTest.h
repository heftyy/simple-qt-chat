#pragma once

#include <gtest/gtest.h>

#include <server/TcpChatServer.h>
#include <client/TcpChatClient.h>
#include <chat/Chatroom.h>

using namespace SimpleChat;

class ClientServerTest : public ::testing::Test {
protected:
    virtual void SetUp() override {
        int argc = 1;
        char *argv[] = {"client_server_test"};

        app = new QApplication(argc, argv);

        secret = "super_secret";
        server = new TcpChatServer(secret, app);
        server->listen(4441, QHostAddress::Any);

        client = new TcpChatClient(app);
    }

    virtual void TearDown() override {
        delete server;
        delete client;
        delete app;
    }

    TcpChatServer* server;
    TcpChatClient* client;
    QApplication* app;

    std::string secret;
};