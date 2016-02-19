#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <chat/Chatee.h>
#include <User.pb.h>
#include <communication/Message.h>

#include "resources/ChatServerTest.h"
#include "resources/ChatCommandTest.h"
#include "resources/MessageTest.h"
#include "mocks/MockChatConnection.h"
#include "mocks/MockChatee.h"

using namespace SimpleChat;

using ::testing::_;
using ::testing::Ne;
using ::testing::Return;
using ::testing::Property;
using ::testing::Eq;
using ::testing::AllOf;

TEST_F(ChatServerTest, IsInitialized) {
    EXPECT_NE(server, nullptr);
}

TEST_F(ChatServerTest, UserJoinRequestTest) {
    using ::testing::Ne;

    auto connection = new MockChatConnection;
    EXPECT_CALL(*connection,
                sendMessageProxy(Ne(nullptr))).Times(2);
    EXPECT_CALL(*connection,
                setChatee(Ne(nullptr))).Times(1);

    server->receiveMessage(MessageTest::validUserJoinRequest(), connection);
}

TEST_F(ChatServerTest, UserListRequestTest) {
    User user;
    user.set_id(1);
    user.set_name("first_user");

    auto connection = new MockChatConnection;
    auto chatee = new MockChatee(user, connection, server->chatroom());

    EXPECT_CALL(*chatee,
                sendMessageProxy(Property(&AbstractMessage::type, Eq(USER_LIST_RESPONSE)))
    ).Times(1);

    server->receiveMessage(MessageTest::validUserListRequest(), std::shared_ptr<MockChatee>(chatee));
}

TEST_F(ChatServerTest, UserChangeTest) {
    User user;
    user.set_id(1);
    user.set_name("first_user");

    auto connection = new MockChatConnection;
    auto chatee = std::make_shared<MockChatee>(user, connection, server->chatroom());

    EXPECT_CALL(*chatee,
                sendMessageProxy(Property(&AbstractMessage::type, Eq(USER_CHANGE)))
    ).Times(1);

    // get the first entry in tuple which is bool success
    EXPECT_TRUE(std::get<0>(server->chatroom()->chateeJoined(chatee)));
    EXPECT_FALSE(std::get<0>(server->chatroom()->chateeJoined(chatee)));

    server->receiveMessage(MessageTest::validUserChange(), chatee);
}

TEST_F(ChatServerTest, ChatMessageTest) {
    User user;
    user.set_id(1);
    user.set_name("first_user");

    auto connection = new MockChatConnection;
    auto chatee = std::make_shared<MockChatee>(user, connection, server->chatroom());

    EXPECT_CALL(*chatee,
                sendChatMessage(Eq("text_message1"), Eq("first_user"), Eq("first_user"))
    ).Times(1);

    EXPECT_CALL(*chatee,
                sendChatMessage(Eq("text_message1"), Eq("first_user"), Eq(""))
    ).Times(1);

    EXPECT_CALL(*chatee,
                sendMessageProxy(Property(&AbstractMessage::type, Eq(CHAT_MESSAGE)))
    ).Times(1);

    // get the first entry in tuple which is bool success
    EXPECT_TRUE(std::get<0>(server->chatroom()->chateeJoined(chatee)));
    EXPECT_FALSE(std::get<0>(server->chatroom()->chateeJoined(chatee)));

    server->receiveMessage(MessageTest::validChatMessage(), chatee);

    auto target = server->chatroom()->getTarget("first_user");
    auto whisperMessage = MessageTest::validChatMessage();
    whisperMessage->set_allocated_target(target.release());
    server->receiveMessage(std::move(whisperMessage), chatee);
}

TEST_F(ChatServerTest, ChatCommandTest) {
    User user;
    user.set_id(1);
    user.set_name("first_user");

    auto connection = new MockChatConnection;
    auto chatee = std::make_shared<MockChatee>(user, connection, server->chatroom());

    EXPECT_CALL(*connection,
                disconnectFromHost()).Times(1);

    EXPECT_CALL(*chatee,
                sendResponse(Eq(true), Eq("auth successful"))
    ).Times(1);

    EXPECT_CALL(*chatee,
                kick(Eq(true))
    ).Times(1);

    EXPECT_CALL(*chatee,
                mute(Eq(true))
    ).Times(1);

    EXPECT_CALL(*chatee,
                unmute(Eq(true))
    ).Times(1);

    // get the first entry in tuple which is bool success
    EXPECT_TRUE(std::get<0>(server->chatroom()->chateeJoined(chatee)));
    EXPECT_FALSE(std::get<0>(server->chatroom()->chateeJoined(chatee)));

    server->receiveMessage(ChatCommandTest::getCommand("/auth " + secret), chatee);
    server->receiveMessage(ChatCommandTest::getCommand("/mute first_user"), chatee);
    server->receiveMessage(ChatCommandTest::getCommand("/unmute first_user"), chatee);
    server->receiveMessage(ChatCommandTest::getCommand("/kick first_user"), chatee);
}