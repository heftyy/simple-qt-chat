#pragma once

#include <memory>
#include <QtNetwork>

namespace SimpleChat {

class MessageDeserializer;

class UserJoinRequest;
class UserJoinResponse;

class UserListRequest;
class UserListResponse;

class UserChange;

class ChatMessage;
class ChatAuthorize;
class ChatCommand;

class ChatConnection;
class Chatee;

class Server : public QObject {
public:
    virtual void listen(quint16 port, QHostAddress ipAddress = QHostAddress::LocalHost) = 0;

    virtual void handleUntypedMessage(const MessageDeserializer& deserializer,
                                      const std::shared_ptr<ChatConnection>& connection) = 0;

    virtual QHostAddress getAddress() const = 0;
    virtual ~Server() { }

private:
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