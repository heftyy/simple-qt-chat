#pragma once

#include <gtest/gtest.h>
#include <client/ChatClient.h>

#include "../mocks/MockClient.h"

class ChatClientTest : public ::testing::Test {
protected:
    virtual void SetUp() override {
        client = new MockClient();
    }

    virtual void TearDown() override {
        delete client;
    }

    MockClient* client;
};