#include <gtest/gtest.h>

#include "resources/DeserializerTest.h"

using namespace SimpleChat;

TEST_F(DeserializerTest, DeserializerWorks) {
    /*
    EXPECT_TRUE(deserializerSuccess.isInitialized());

    EXPECT_EQ(deserializerSuccess.type(), SimpleChat::CHAT_MESSAGE);

    auto msg = deserializerSuccess.getMessage<ChatMessage>();
    EXPECT_TRUE(msg->IsInitialized());
    EXPECT_EQ(msg->text(), "text_message1");
     */
}

TEST_F(DeserializerTest, DeserializerFails) {
    /*
    EXPECT_FALSE(deserializerFail.isInitialized());

    EXPECT_EQ(deserializerFail.type(), -1);

    auto msg = deserializerFail.getMessage<ChatMessage>();

    EXPECT_EQ(msg, nullptr);
     */
}