#pragma once

#include <gtest/gtest.h>
#include <server/ChatServer.h>
#include <chat/Chatroom.h>

class ChatServerTest : public ::testing::Test {
protected:
    virtual void SetUp() override {
        secret = "super_secret";
        server = new SimpleChat::ChatServer(secret);
    }

    virtual void TearDown() override {
        delete server;
    }

    SimpleChat::ChatServer* server;
    std::string secret;
};