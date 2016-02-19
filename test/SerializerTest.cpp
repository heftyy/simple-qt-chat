#include <gtest/gtest.h>

#include "resources/MessageTest.h"
#include "resources/AbstractMessageTest.h"
#include "resources/SerializerTest.h"

TEST_F(SerializerTest, SerializerWorks) {
    bool success;
    std::string result;

    std::tie(success, result) = getSerializer(AbstractMessageTest::build(MessageTest::validChatCommand())).serialize();
    EXPECT_TRUE(success);
    std::tie(success, result) = getSerializer(AbstractMessageTest::build(MessageTest::validChatResponse())).serialize();
    EXPECT_TRUE(success);
    std::tie(success, result) = getSerializer(AbstractMessageTest::build(MessageTest::validChatroomChange())).serialize();
    EXPECT_TRUE(success);
    std::tie(success, result) = getSerializer(AbstractMessageTest::build(MessageTest::validChatMessage())).serialize();
    EXPECT_TRUE(success);
    std::tie(success, result) = getSerializer(AbstractMessageTest::build(MessageTest::validUserChange())).serialize();
    EXPECT_TRUE(success);
    std::tie(success, result) = getSerializer(AbstractMessageTest::build(MessageTest::validUserListRequest())).serialize();
    EXPECT_TRUE(success);
    std::tie(success, result) = getSerializer(AbstractMessageTest::build(MessageTest::validUserListResponse())).serialize();
    EXPECT_TRUE(success);
    std::tie(success, result) = getSerializer(AbstractMessageTest::build(MessageTest::validUserJoinRequest())).serialize();
    EXPECT_TRUE(success);
    std::tie(success, result) = getSerializer(AbstractMessageTest::build(MessageTest::validUserJoinResponse())).serialize();
    EXPECT_TRUE(success);
}

TEST_F(SerializerTest, SerializerFails) {
    bool success;
    std::string result;

    std::tie(success, result) = getSerializer(AbstractMessageTest::build(MessageTest::invalidChatCommand())).serialize();
    EXPECT_FALSE(success);
    std::tie(success, result) = getSerializer(AbstractMessageTest::build(MessageTest::invalidChatResponse())).serialize();
    EXPECT_FALSE(success);
    std::tie(success, result) = getSerializer(AbstractMessageTest::build(MessageTest::invalidChatMessage())).serialize();
    EXPECT_FALSE(success);
    std::tie(success, result) = getSerializer(AbstractMessageTest::build(MessageTest::invalidUserChange())).serialize();
    EXPECT_FALSE(success);
    std::tie(success, result) = getSerializer(AbstractMessageTest::build(MessageTest::invalidUserListRequest())).serialize();
    EXPECT_FALSE(success);
//    this never fails because UserListResponse has only optional fields
//    std::tie(success, result) = getSerializer(AbstractMessageTest::build(MessageTest::invalidUserListResponse())).serialize();
//    EXPECT_FALSE(success);
    std::tie(success, result) = getSerializer(AbstractMessageTest::build(MessageTest::invalidUserJoinRequest())).serialize();
    EXPECT_FALSE(success);
    std::tie(success, result) = getSerializer(AbstractMessageTest::build(MessageTest::invalidUserJoinResponse())).serialize();
    EXPECT_FALSE(success);
}