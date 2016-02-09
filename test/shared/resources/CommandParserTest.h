#pragma once

#include <gtest/gtest.h>

#include "ChatMessage.pb.h"

using namespace SimpleChat;

class CommandParserTest : public ::testing::Test {
protected:
    virtual void SetUp() override {
        chatTarget = new ChatTarget();
        chatTarget->set_user_id(1);
        chatTarget->set_user_name("first user");
    }

    virtual void TearDown() override {
    }

    ChatTarget* chatTarget;
};