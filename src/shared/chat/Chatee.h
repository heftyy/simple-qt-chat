#pragma once

#include <memory>
#include <string>

#include <google/protobuf/message.h>

namespace SimpleChat {

class User;
class Chatroom;
class ChatConnection;
class ChatTarget;
class AbstractMessage;

class Chatee
{
public:
    explicit Chatee(std::unique_ptr<User> user,
                    const std::shared_ptr<ChatConnection>& connection);

    virtual ~Chatee();

    User& user();

    virtual void setAuthorized(bool authorized) {};

    virtual bool sendMessage(std::unique_ptr<AbstractMessage> message);

    void sendMessage(const std::string& message, const std::string& target = "");
    void sendCommand(const std::string& command);
    void authorize(const std::string& password);

private:
    std::unique_ptr<User> user_;
    std::shared_ptr<ChatConnection> connection_;
    std::weak_ptr<Chatroom> chatroom_;

    virtual std::unique_ptr<ChatTarget> getSelf();
    virtual std::unique_ptr<ChatTarget> getTarget(const std::string& target);

    template<typename MessageType>
    void prepareAndSend(std::unique_ptr<MessageType> message, int type);
};

} // SimpleChat namespace