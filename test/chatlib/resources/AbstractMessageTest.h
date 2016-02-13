#pragma once

#include <gtest/gtest.h>
#include <communication/Message.h>
#include <ChatMessage.pb.h>

class AbstractMessageTest : public ::testing::Test {
    using am = SimpleChat::AbstractMessage;
    using m = SimpleChat::Message<SimpleChat::ChatMessage>;

protected:
    virtual void SetUp() override {}

    virtual void TearDown() override {}

public:
    static std::unique_ptr<am> invalidAbstractMessage() {
        auto message = std::make_unique<SimpleChat::ChatMessage>();

        return std::make_unique<m>(std::move(message), SimpleChat::USER_JOIN_REQUEST);
    }

    static std::unique_ptr<am> validAbstractMessage() {
        auto message = std::make_unique<SimpleChat::ChatMessage>();
        message->set_text("text_message1");

        return std::make_unique<m>(std::move(message), SimpleChat::USER_JOIN_REQUEST);
    }
};