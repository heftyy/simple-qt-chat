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

    virtual bool sendMessage(std::unique_ptr<AbstractMessage> message);

    void sendChatMessage(const std::string& message, const std::string& from, const std::string& target = "");
    void sendResponse(bool success, const std::string& message);

    void mute();
    void kick();

    std::shared_ptr<ChatConnection> connection() const;

    bool authorized() const;
    void setAuthorized(bool authorized);

private:
    std::unique_ptr<User> user_;
    std::shared_ptr<ChatConnection> connection_;
    std::weak_ptr<Chatroom> chatroom_;
    bool authorized_;

    virtual std::unique_ptr<ChatTarget> getSelf();
    virtual std::unique_ptr<ChatTarget> getTarget(const std::string& target);

    template<typename MessageType>
    void prepareAndSend(std::unique_ptr<MessageType> message, int type);
};

} // SimpleChat namespace