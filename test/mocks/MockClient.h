#pragma once

#include <string>

#include <gmock/gmock.h>

#include <client/ChatClient.h>

using namespace SimpleChat;

class MockClient : public ChatClient {
public:
    /*
     *
    virtual bool connectToServer() = 0;
    virtual void join() = 0;
    virtual void requestUserList() = 0;

    virtual bool sendAnyMessage(std::unique_ptr<AbstractMessage> message) override = 0;
    virtual bool isConnected() override = 0;
    virtual ChatConnection* connection() override = 0;

    // event definitions
    virtual void chateeJoined(const std::string& name) override = 0;
    virtual void chateeLeft(const std::string& name) override = 0;
    virtual void chatMotdChanged(const std::string& motd) override = 0;
    virtual void chatInfoReceived(const std::string& info) override = 0;
    virtual void chatMessageReceived(const std::string& text,
                                     const std::string& from,
                                     const std::string& target) override = 0;

    virtual void refreshChateeList() override = 0;
     */

    MOCK_METHOD0(connectToServer, bool());
    MOCK_METHOD0(join, void());
    MOCK_METHOD0(requestUserList, void());

    MOCK_METHOD1(sendAnyMessageProxy, bool(AbstractMessage*));

    bool sendAnyMessage(std::unique_ptr<AbstractMessage> mPtr) {
        return sendAnyMessageProxy(mPtr.get());
    }

    MOCK_METHOD0(isConnected, bool());
    MOCK_METHOD0(connection, ChatConnection*());

    MOCK_METHOD1(chateeJoined, void(const std::string&));
    MOCK_METHOD1(chateeLeft, void(const std::string&));
    MOCK_METHOD1(chatMotdChanged, void(const std::string&));
    MOCK_METHOD1(chatInfoReceived, void(const std::string&));
    MOCK_METHOD3(chatMessageReceived, void(const std::string&, const std::string&, const std::string&));

    MOCK_METHOD0(refreshChateeList, void());
};
