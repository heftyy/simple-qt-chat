#pragma once

#include <gtest/gtest.h>
#include <communication/Message.h>
#include <ChatMessage.pb.h>

using namespace SimpleChat;

class AbstractMessageTest : public ::testing::Test {
    using am = SimpleChat::AbstractMessage;
    using m = SimpleChat::Message<SimpleChat::ChatMessage>;

protected:
    virtual void SetUp() override {}

    virtual void TearDown() override {}

public:
    static std::unique_ptr<am> invalidChatMessage() {
        auto message = std::make_unique<SimpleChat::ChatMessage>();

        return MessageBuilder::build(std::move(message));
    }

    static std::unique_ptr<am> validChatMessage() {
        auto message = std::make_unique<SimpleChat::ChatMessage>();
        message->set_text("text_message1");

        return MessageBuilder::build(std::move(message));
    }
};