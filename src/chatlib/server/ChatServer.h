#pragma once

#include <memory>

#include "Server.h"

namespace SimpleChat {

class Chatroom;

class ChatServer : public Server {
public:
    explicit ChatServer(const std::string& password);

    virtual void chateeLeft(const std::shared_ptr<Chatee>& chatee) override;

    virtual void handleUntypedMessage(const MessageDeserializer& deserializer,
                                      ChatConnection* connection) override;

    virtual void handleMessage(std::unique_ptr<UserJoinRequest> joinRequest,
                               ChatConnection* connection) override;

    virtual void handleMessage(std::unique_ptr<UserListRequest> listRequest,
                               const std::shared_ptr<Chatee>& sender) override;

    virtual void handleMessage(std::unique_ptr<UserChange> userChange,
                               const std::shared_ptr<Chatee>& sender) override;

    virtual void handleMessage(std::unique_ptr<ChatMessage> chatMessage,
                               const std::shared_ptr<Chatee>& sender) override;

    virtual void handleMessage(std::unique_ptr<ChatCommand> chatCommand,
                               const std::shared_ptr<Chatee>& sender) override;

protected:
    std::shared_ptr<Chatroom> chatroom_;
    std::string password_;
};

} // SimpleChat namespace
