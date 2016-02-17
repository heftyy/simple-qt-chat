#pragma once

#include <gtest/gtest.h>
#include <communication/Message.h>
#include <ChatMessage.pb.h>
#include <User.pb.h>

#include "ChatCommandTest.h"

using namespace SimpleChat;

class AbstractMessageTest : public ::testing::Test {
    using am = SimpleChat::AbstractMessage;
    using amPtr = std::unique_ptr<am>;

protected:
    virtual void SetUp() override {}

    virtual void TearDown() override {}

public:
    template<typename T>
    static amPtr build(T&& msg) {
        return MessageBuilder::build(std::forward<T>(msg));
    }
};