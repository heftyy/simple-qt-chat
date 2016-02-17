#pragma once

#include <string>

#include <gmock/gmock.h>

#include <chat/Chatee.h>

using namespace SimpleChat;

class MockChatee : public Chatee {
public:
    explicit MockChatee(const User& user,
                    ChatConnection* connection,
                    const std::shared_ptr<Chatroom>& chatroom) : Chatee(user, connection, chatroom) {};

    bool sendMessage(std::unique_ptr<AbstractMessage> mPtr) {
        return sendMessageProxy(mPtr.get());
    }

    MOCK_METHOD1(sendMessageProxy, bool(AbstractMessage*));

    MOCK_METHOD3(sendChatMessage, void(const std::string&, const std::string&, const std::string&));
    MOCK_METHOD2(sendResponse, void(bool, const std::string&));

    MOCK_METHOD1(mute, void(bool));
    MOCK_METHOD1(unmute, void(bool));
    MOCK_METHOD1(kick, void(bool));

    MOCK_CONST_METHOD0(connection, ChatConnection*());
    MOCK_CONST_METHOD0(authorized, bool());
    MOCK_METHOD1(setAuthorized, void(bool));
};
