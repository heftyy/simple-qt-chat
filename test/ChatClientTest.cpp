#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <chat/Chatee.h>
#include <chat/Chatroom.h>

#include "mocks/MockChatConnection.h"
#include "mocks/MockChatee.h"

#include "resources/ChatClientTest.h"
#include "resources/MessageTest.h"

using namespace SimpleChat;

using ::testing::_;
using ::testing::Ne;
using ::testing::Return;
using ::testing::Property;
using ::testing::Eq;
using ::testing::AllOf;

TEST_F(ChatClientTest, IsInitialized) {
    EXPECT_NE(client, nullptr);
}

TEST_F(ChatClientTest, UserJoinResponseTest) {
    EXPECT_CALL(*client,
                chatMotdChanged(_)).Times(1);

    EXPECT_CALL(*client,
                requestUserList()).Times(1);

    EXPECT_CALL(*client,
                connection()).Times(1);

    client->receiveMessage(MessageTest::validUserJoinResponse());
}

TEST_F(ChatClientTest, UserListResponseTest) {
    EXPECT_CALL(*client,
                refreshChateeList()).Times(1);

    EXPECT_CALL(*client,
                connection()).Times(1);

    client->receiveMessage(MessageTest::validUserListResponse());
}

TEST_F(ChatClientTest, UserChangeTest) {
    User user;
    user.set_id(1);
    user.set_name("my_first_user");

    auto connection = new MockChatConnection;
    auto chatee = std::make_shared<MockChatee>(user, connection, client->chatroom());

    EXPECT_TRUE(std::get<0>(client->chatroom()->chateeJoined(chatee)));

    EXPECT_CALL(*client,
                chatInfoReceived(_)).Times(1);

    EXPECT_CALL(*client,
                refreshChateeList()).Times(1);

    client->receiveMessage(MessageTest::validUserChange());
}

TEST_F(ChatClientTest, ChatMessageTest) {
    EXPECT_CALL(*client,
                chatMessageReceived(_, _, _)).Times(1);

    client->receiveMessage(MessageTest::validChatMessage());
}

TEST_F(ChatClientTest, ChatroomChangeTest) {
    EXPECT_CALL(*client,
                chatMotdChanged(_)).Times(1);

    client->receiveMessage(MessageTest::validChatroomChange());
}

TEST_F(ChatClientTest, GenericChatResponseTest) {
    EXPECT_CALL(*client,
                chatInfoReceived(_)).Times(1);

    client->receiveMessage(MessageTest::validChatResponse());
}