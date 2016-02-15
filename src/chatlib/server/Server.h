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

/*!
 * Interface for all chat servers.
 */
class Server {
public:
    virtual ~Server() {}

    virtual void chateeLeft(const std::shared_ptr<Chatee>& chatee) = 0;

    virtual void receiveUntypedMessage(const MessageDeserializer& deserializer,
                                      ChatConnection* connection) = 0;

    virtual void receiveMessage(std::unique_ptr<UserJoinRequest> joinRequest,
                               ChatConnection* connection) = 0;

    virtual void receiveMessage(std::unique_ptr<UserListRequest> listRequest,
                               const std::shared_ptr<Chatee>& sender) = 0;

    virtual void receiveMessage(std::unique_ptr<UserChange> change,
                               const std::shared_ptr<Chatee>& sender) = 0;

    virtual void receiveMessage(std::unique_ptr<ChatMessage> chatMessage,
                               const std::shared_ptr<Chatee>& sender) = 0;

    virtual void receiveMessage(std::unique_ptr<ChatCommand> chatCommand,
                               const std::shared_ptr<Chatee>& sender) = 0;
};

} // SimpleChat namespace
