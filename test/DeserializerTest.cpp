#include <gtest/gtest.h>
#include "resources/AbstractMessageTest.h"
#include "resources/MessageTest.h"

#include "resources/DeserializerTest.h"

using namespace SimpleChat;

TEST_F(DeserializerTest, DeserializerWorks) {
    deserializerWorks<ChatCommand>(AbstractMessageTest::build(MessageTest::validChatCommand()), CHAT_COMMAND);
    deserializerWorks<GenericChatResponse>(AbstractMessageTest::build(MessageTest::validChatResponse()), GENERIC_CHAT_RESPONSE);
    deserializerWorks<ChatroomChange>(AbstractMessageTest::build(MessageTest::validChatroomChange()), CHATROOM_CHANGE);
    deserializerWorks<ChatMessage>(AbstractMessageTest::build(MessageTest::validChatMessage()), CHAT_MESSAGE);
    deserializerWorks<UserChange>(AbstractMessageTest::build(MessageTest::validUserChange()), USER_CHANGE);
    deserializerWorks<UserListRequest>(AbstractMessageTest::build(MessageTest::validUserListRequest()), USER_LIST_REQUEST);
    deserializerWorks<UserListResponse>(AbstractMessageTest::build(MessageTest::validUserListResponse()), USER_LIST_RESPONSE);
    deserializerWorks<UserJoinRequest>(AbstractMessageTest::build(MessageTest::validUserJoinRequest()), USER_JOIN_REQUEST);
    deserializerWorks<UserJoinResponse>(AbstractMessageTest::build(MessageTest::validUserJoinResponse()), USER_JOIN_RESPONSE);
}

TEST_F(DeserializerTest, DeserializerFails) {
    deserializerFails<ChatCommand>(AbstractMessageTest::build(MessageTest::invalidChatCommand()));
    deserializerFails<GenericChatResponse>(AbstractMessageTest::build(MessageTest::invalidChatResponse()));
    deserializerFails<ChatMessage>(AbstractMessageTest::build(MessageTest::invalidChatMessage()));
    deserializerFails<UserChange>(AbstractMessageTest::build(MessageTest::invalidUserChange()));
    deserializerFails<UserListRequest>(AbstractMessageTest::build(MessageTest::invalidUserListRequest()));
//    this never fails because UserListResponse has only optional fields
//    deserializerFails<UserListResponse>(AbstractMessageTest::build(MessageTest::invalidUserListResponse()));
    deserializerFails<UserJoinRequest>(AbstractMessageTest::build(MessageTest::invalidUserJoinRequest()));
    deserializerFails<UserJoinResponse>(AbstractMessageTest::build(MessageTest::invalidUserJoinResponse()));
}