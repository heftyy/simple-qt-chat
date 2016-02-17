#pragma once

#include <gtest/gtest.h>
#include <communication/Message.h>
#include <ChatMessage.pb.h>
#include <User.pb.h>

#include "ChatCommandTest.h"

using namespace SimpleChat;

class MessageTest : public ::testing::Test {
protected:
    virtual void SetUp() override {}

    virtual void TearDown() override {}

public:
    static std::unique_ptr<ChatMessage> invalidChatMessage() {
        auto message = std::make_unique<SimpleChat::ChatMessage>();

        return std::move(message);
    }

    static std::unique_ptr<ChatMessage> validChatMessage() {
        auto message = std::make_unique<SimpleChat::ChatMessage>();
        message->set_text("text_message1");

        return std::move(message);
    }

    static std::unique_ptr<ChatCommand> invalidChatCommand() {
        auto message = std::make_unique<SimpleChat::ChatCommand>();

        return std::move(message);
    }

    static std::unique_ptr<ChatCommand> validChatCommand() {
        auto command = ChatCommandTest::getCommand("/auth magical_password");

        return std::move(command);
    }

    static std::unique_ptr<ChatroomChange> validChatroomChange() {
        auto chatroomChange = std::make_unique<SimpleChat::ChatroomChange>();
        chatroomChange->set_motd("fantastic motd");

        return std::move(chatroomChange);
    }

    static std::unique_ptr<GenericChatResponse> invalidChatResponse() {
        auto response = std::make_unique<SimpleChat::GenericChatResponse>();

        return std::move(response);
    }

    static std::unique_ptr<GenericChatResponse> validChatResponse() {
        auto response = std::make_unique<SimpleChat::GenericChatResponse>();
        response->set_success(true);
        response->set_message("success!");

        return std::move(response);
    }

    static std::unique_ptr<UserJoinRequest> invalidUserJoinRequest() {
        auto request = std::make_unique<SimpleChat::UserJoinRequest>();

        return std::move(request);
    }

    static std::unique_ptr<UserJoinRequest> validUserJoinRequest() {
        auto request = std::make_unique<SimpleChat::UserJoinRequest>();
        request->set_name("my_name");

        return std::move(request);
    }

    static std::unique_ptr<UserJoinResponse> invalidUserJoinResponse() {
        auto response = std::make_unique<SimpleChat::UserJoinResponse>();

        return std::move(response);
    }

    static std::unique_ptr<UserJoinResponse> validUserJoinResponse() {
        auto response = std::make_unique<SimpleChat::UserJoinResponse>();
        response->set_success(true);
        response->set_motd("best motd");
        response->set_message("joined successfully");
        auto user = response->mutable_user();
        user->set_id(1);
        user->set_name("my_user_name");
        user->set_presence(ONLINE);

        return std::move(response);
    }

    static std::unique_ptr<UserListRequest> invalidUserListRequest() {
        auto request = std::make_unique<SimpleChat::UserListRequest>();

        return std::move(request);
    }

    static std::unique_ptr<UserListRequest> validUserListRequest() {
        auto request = std::make_unique<SimpleChat::UserListRequest>();
        request->set_name("requester_name");

        return std::move(request);
    }

    static std::unique_ptr<UserListResponse> invalidUserListResponse() {
        auto response = std::make_unique<SimpleChat::UserListResponse>();

        return std::move(response);
    }

    static std::unique_ptr<UserListResponse> validUserListResponse() {
        auto response = std::make_unique<SimpleChat::UserListResponse>();
        auto user = response->mutable_users()->Add();
        user->set_id(1);
        user->set_name("my_first_user");
        user->set_presence(ONLINE);

        return std::move(response);
    }

    static std::unique_ptr<UserChange> invalidUserChange() {
        auto change = std::make_unique<SimpleChat::UserChange>();

        return std::move(change);
    }

    static std::unique_ptr<UserChange> validUserChange() {
        auto change = std::make_unique<SimpleChat::UserChange>();
        auto user = change->mutable_user();
        user->set_id(1);
        user->set_name("my_first_user");
        user->set_presence(ONLINE);
        change->set_presence(UserPresence::AWAY);

        return std::move(change);
    }

};