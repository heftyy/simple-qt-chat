#pragma once

#include <gtest/gtest.h>
#include <server/ChatServer.h>
#include <chat/Chatroom.h>

class ChatServerTest : public ::testing::Test {
protected:
    virtual void SetUp() override {
        password = "super_secret";
        server = new SimpleChat::ChatServer(password);
    }

    virtual void TearDown() override {
        delete server;
    }

    SimpleChat::ChatServer* server;
    std::string password;
};