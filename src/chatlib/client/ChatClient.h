#pragma once

#include <ChatMessage.pb.h>
#include "Client.h"

namespace SimpleChat {

class MessageDeserializer;

class UserJoinResponse;
class UserListResponse;

class UserChange;

class ChatMessage;
class ChatAuthorize;
class ChatCommand;

class ChatConnection;
class Chatroom;

class ChatClient : public Client {
public:
    ChatClient();

    virtual void sendCommand(const std::string& command) override;
    virtual void sendMessage(const std::string& text, const std::string& target = "") override;

    virtual void handleUntypedMessage(const MessageDeserializer& deserializer) override;

    virtual void handleMessage(std::unique_ptr<UserJoinResponse> joinResponse) override;
    virtual void handleMessage(std::unique_ptr<UserListResponse> listResponse) override;
    virtual void handleMessage(std::unique_ptr<UserChange> userChange) override;
    virtual void handleMessage(std::unique_ptr<ChatMessage> chatMessage) override;
    virtual void handleMessage(std::unique_ptr<ChatroomChange> chatroomChange) override;
    virtual void handleMessage(std::unique_ptr<GenericChatResponse> response) override;

    virtual bool connectToServer() = 0;
    virtual void join() = 0;
    virtual void requestUserList() = 0;

protected:
    std::string clientName_;
    std::shared_ptr<Chatroom> chatroom_;

    virtual bool sendAnyMessage(std::unique_ptr<AbstractMessage> message) override = 0;
    virtual bool isConnected() override = 0;
    virtual ChatConnection* connection() override = 0;

    virtual void chatMotdChanged(const std::string& motd) override = 0;
    virtual void chatInfoReceived(const std::string& info) override = 0;
    virtual void chatMessageReceived(const std::string& text, 
                             const std::string& from, 
                             const std::string& target) override = 0;

    virtual void refreshChateeList() override = 0;
    
};

} // SimpleChat namespace
