#pragma once

#include <string>

#include <gmock/gmock.h>

#include <communication/AbstractMessage.h>
#include <chat/ChatConnection.h>

using namespace SimpleChat;

class MockChatConnection : public ChatConnection {
public:
    bool sendMessage(std::unique_ptr<AbstractMessage> mPtr) {
        return sendMessageProxy(mPtr.get());
    }

    /*
    virtual std::unique_ptr<AbstractMessage> nonCopyableReturn() {
        return std::unique_ptr<AbstractMessage>(nonCopyableReturnProxy());
    }
    MOCK_METHOD0(nonCopyableReturnProxy,IMyObjectThing* ());
    */

    MOCK_METHOD1(sendMessageProxy, bool(AbstractMessage*));
    MOCK_CONST_METHOD0(isAlive, bool());
    MOCK_CONST_METHOD0(getIdent, std::string());

    MOCK_METHOD0(disconnectFromHost, void());

    MOCK_METHOD1(setChatee, void(const std::shared_ptr<Chatee>&));
    MOCK_CONST_METHOD0(chatee, std::shared_ptr<Chatee>());
};
