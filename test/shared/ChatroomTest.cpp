#include <gtest/gtest.h>

#include <chat/Chatee.h>
#include <User.pb.h>

#include "resources/ChatroomTest.h"
#include "../mocks/MockChatConnection.h"

using namespace SimpleChat;

TEST_F(ChatroomTest, IsInitialized) {
    EXPECT_NE(room1, nullptr);
}

TEST_F(ChatroomTest, AddChatee) {
    bool success;
    std::string message;
    std::shared_ptr<Chatee> chatee;
    std::tie(success, message, chatee) = room1->chateeJoined("first_chatee", nullptr);

    EXPECT_TRUE(success);
    EXPECT_EQ(message, "");
    EXPECT_NE(chatee, nullptr);

    EXPECT_TRUE(room1->chateeExists("first_chatee"));

    chatee = room1->getChatee("first_chatee");
    EXPECT_EQ(chatee->user().name(), "first_chatee");

    std::tie(success, message, chatee) = room1->chateeJoined("first_chatee", nullptr);
    EXPECT_FALSE(success);
    EXPECT_EQ(message, "That name is already taken");
    EXPECT_EQ(chatee, nullptr);
}

TEST_F(ChatroomTest, DeleteChatee) {
    bool success;
    std::string message;
    std::shared_ptr<SimpleChat::Chatee> chatee;
    std::tie(success, message, chatee) = room1->chateeJoined("first_chatee", nullptr);

    EXPECT_TRUE(success);
    EXPECT_EQ(message, "");
    EXPECT_NE(chatee, nullptr);

    std::tie(success, message) = room1->chateeLeft("first_chatee");
    EXPECT_TRUE(success);
    EXPECT_EQ(message, "");

    EXPECT_FALSE(room1->chateeExists("first_chatee"));

    chatee = room1->getChatee("first_chatee");
    EXPECT_EQ(chatee, nullptr);

    std::tie(success, message, chatee) = room1->chateeJoined("first_chatee", nullptr);
    EXPECT_TRUE(success);
    EXPECT_EQ(message, "");
    EXPECT_NE(chatee, nullptr);
}

TEST_F(ChatroomTest, TestMessageSend) {
    using ::testing::_;

    auto connection = std::make_shared<MockChatConnection>();
    EXPECT_CALL(*connection, sendMessageProxy(_)).Times(1);

    bool success;
    std::string message;
    std::shared_ptr<SimpleChat::Chatee> chatee;
    std::tie(success, message, chatee) = room1->chateeJoined("first_chatee", connection);

    chatee->sendMessage("first message");
}