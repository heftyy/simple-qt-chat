#include <gtest/gtest.h>

#include "resources/DeserializerTest.h"

using namespace SimpleChat;

TEST_F(DeserializerTest, IsInitialized) {
    EXPECT_NE(deserializerSuccess, nullptr);
    EXPECT_NE(deserializerFail, nullptr);
}

TEST_F(DeserializerTest, DeserializerWorks) {
    EXPECT_TRUE(deserializerSuccess->isInitialized());

    EXPECT_EQ(deserializerSuccess->type(), SimpleChat::USER_JOIN_REQUEST);

    auto msg = deserializerSuccess->getMessage<ChatMessage>();
    EXPECT_TRUE(msg->IsInitialized());
    EXPECT_EQ(msg->text(), "text_message1");
}

TEST_F(DeserializerTest, DeserializerFails) {
    EXPECT_FALSE(deserializerFail->isInitialized());

    EXPECT_EQ(deserializerFail->type(), -1);

    auto msg = deserializerFail->getMessage<ChatMessage>();

    EXPECT_EQ(msg, nullptr);
}