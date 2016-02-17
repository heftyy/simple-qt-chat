#include <gtest/gtest.h>

#include "resources/MessageTest.h"
#include "resources/AbstractMessageTest.h"

TEST_F(AbstractMessageTest, InvalidMessage) {
    EXPECT_FALSE(AbstractMessageTest::build(MessageTest::invalidChatMessage())->isInitialized());
    EXPECT_FALSE(AbstractMessageTest::build(MessageTest::invalidChatCommand())->isInitialized());
    EXPECT_FALSE(AbstractMessageTest::build(MessageTest::invalidChatResponse())->isInitialized());

    EXPECT_FALSE(AbstractMessageTest::build(MessageTest::invalidUserJoinRequest())->isInitialized());
    EXPECT_FALSE(AbstractMessageTest::build(MessageTest::invalidUserJoinResponse())->isInitialized());
    EXPECT_FALSE(AbstractMessageTest::build(MessageTest::invalidUserListRequest())->isInitialized());
    EXPECT_FALSE(AbstractMessageTest::build(MessageTest::invalidUserChange())->isInitialized());
}

TEST_F(AbstractMessageTest, ValidMessage) {
    EXPECT_TRUE(AbstractMessageTest::build(MessageTest::validChatMessage())->isInitialized());
    EXPECT_TRUE(AbstractMessageTest::build(MessageTest::validChatCommand())->isInitialized());
    EXPECT_TRUE(AbstractMessageTest::build(MessageTest::validChatResponse())->isInitialized());
    EXPECT_TRUE(AbstractMessageTest::build(MessageTest::validChatroomChange())->isInitialized());

    EXPECT_TRUE(AbstractMessageTest::build(MessageTest::validUserJoinRequest())->isInitialized());
    EXPECT_TRUE(AbstractMessageTest::build(MessageTest::validUserJoinResponse())->isInitialized());
    EXPECT_TRUE(AbstractMessageTest::build(MessageTest::validUserListRequest())->isInitialized());
    EXPECT_TRUE(AbstractMessageTest::build(MessageTest::validUserListResponse())->isInitialized());
    EXPECT_TRUE(AbstractMessageTest::build(MessageTest::validUserChange())->isInitialized());
}

TEST_F(AbstractMessageTest, CloneMessage) {
    auto msg1 = build(MessageTest::validChatMessage());
    auto msg2 = msg1->clone();

    EXPECT_EQ(msg1->type(), msg2->type());
    EXPECT_EQ(msg1->serialize(), msg2->serialize());
}