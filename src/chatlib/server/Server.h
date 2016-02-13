#pragma once

#include <memory>

namespace SimpleChat {

class UserJoinRequest;
class UserListRequest;
class UserChange;
class ChatMessage;
class ChatAuthorize;
class ChatCommand;

class MessageDeserializer;
class ChatConnection;
class Chatee;

class Server {
public:
    virtual ~Server() {}

    virtual void handleUntypedMessage(const MessageDeserializer& deserializer,
                                      const std::shared_ptr<ChatConnection>& connection) = 0;

    virtual void handleMessage(std::unique_ptr<UserJoinRequest> joinRequest,
                               const std::shared_ptr<ChatConnection>& connection) = 0;

    virtual void handleMessage(std::unique_ptr<UserListRequest> listRequest,
                               const std::shared_ptr<Chatee>& sender) = 0;

    virtual void handleMessage(std::unique_ptr<UserChange> change,
                               const std::shared_ptr<Chatee>& sender) = 0;

    virtual void handleMessage(std::unique_ptr<ChatMessage> chatMessage,
                               const std::shared_ptr<Chatee>& sender) = 0;

    virtual void handleMessage(std::unique_ptr<ChatAuthorize> chatAuthorize,
                               const std::shared_ptr<Chatee>& sender) = 0;

    virtual void handleMessage(std::unique_ptr<ChatCommand> chatCommand,
                               const std::shared_ptr<Chatee>& sender) = 0;
};

} //SimpleChat namespace
