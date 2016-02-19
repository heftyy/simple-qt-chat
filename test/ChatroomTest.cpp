#include <gtest/gtest.h>

#include <chat/Chatee.h>
#include <User.pb.h>
#include <communication/Message.h>

#include "resources/ChatroomTest.h"
#include "mocks/MockChatConnection.h"

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

TEST_F(ChatroomTest, MessageSend) {
//    using ::testing::_;
    using ::testing::Ne;

    auto connection = new MockChatConnection;
    EXPECT_CALL(*connection, sendMessageProxy(Ne(nullptr))).Times(1);
    EXPECT_CALL(*connection, setChatee(Ne(nullptr))).Times(1);

    bool success;
    std::string message;
    std::shared_ptr<SimpleChat::Chatee> chatee;
    std::tie(success, message, chatee) = room1->chateeJoined("first_chatee", connection);

    chatee->sendChatMessage("first message", "first_chatee");

    delete connection;
}

TEST_F(ChatroomTest, PropagateMessage) {
    //    using ::testing::_;
    using ::testing::Ne;
    using ::testing::Return;

    auto connection = new MockChatConnection;
    EXPECT_CALL(*connection, sendMessageProxy(Ne(nullptr))).Times(2).WillRepeatedly(Return(true));
    EXPECT_CALL(*connection, setChatee(Ne(nullptr))).Times(2);

    bool success;
    std::string message;
    std::shared_ptr<SimpleChat::Chatee> chatee;

    std::tie(success, message, chatee) = room1->chateeJoined("first_chatee", connection);
    std::tie(success, message, chatee) = room1->chateeJoined("second_chatee", connection);

    auto msg = std::make_unique<UserChange>();
    msg->mutable_user()->CopyFrom(chatee->user());
    msg->set_action(JOINED);
    room1->propagateMessage(MessageBuilder::build(std::move(msg)));

    delete connection;
}