#include <future>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <QCoreApplication>
#include <QtNetwork>
#include <QTest>
#include <QSignalSpy>

#include <server/TcpChatServer.h>
#include <client/TcpChatClient.h>
#include <chat/Chatroom.h>
#include <chat/Chatee.h>

#include "resources/ClientServerTest.h"

using namespace SimpleChat;

TEST_F(ClientServerTest, LoginTest) {
    EXPECT_EQ(server->chatroom()->map().size(), 0);

    EXPECT_TRUE(client->login("127.0.0.1", 4441, "nickname1"));

    while(app->hasPendingEvents()) {
        app->processEvents();
    }

    EXPECT_EQ(server->chatroom()->map().size(), 1);

    client->logout();

    while(app->hasPendingEvents()) {
        app->processEvents();
    }

    EXPECT_EQ(server->chatroom()->map().size(), 0);
}

TEST_F(ClientServerTest, MessageTest) {
    EXPECT_EQ(server->chatroom()->map().size(), 0);

    EXPECT_TRUE(client->login("127.0.0.1", 4441, "nickname1"));

    QSignalSpy spy(client, SIGNAL(chatMessageSignal(QString, QString, QString)));
    client->sendMessage("msg1");

    while(app->hasPendingEvents()) {
        app->processEvents();
    }

    EXPECT_EQ(spy.count(), 1);

    client->logout();
}

TEST_F(ClientServerTest, CommandTest) {
    EXPECT_EQ(server->chatroom()->map().size(), 0);

    EXPECT_TRUE(client->login("127.0.0.1", 4441, "nickname1"));

    client->sendCommand("/auth " + secret);

    while(app->hasPendingEvents()) {
        app->processEvents();
    }

    EXPECT_TRUE(server->chatroom()->getChatee("nickname1")->authorized());

    client->sendCommand("/mute nickname1");

    while(app->hasPendingEvents()) {
        app->processEvents();
    }

    EXPECT_TRUE(server->chatroom()->getChatee("nickname1")->user().mute());

    client->sendCommand("/unmute nickname1");

    while(app->hasPendingEvents()) {
        app->processEvents();
    }

    EXPECT_FALSE(server->chatroom()->getChatee("nickname1")->user().mute());

    client->sendCommand("/kick nickname1");

    while(app->hasPendingEvents()) {
        app->processEvents();
    }

    EXPECT_EQ(server->chatroom()->getChatee("nickname1"), nullptr);

    client->logout();
}
