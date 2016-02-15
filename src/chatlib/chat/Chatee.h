#pragma once

#include <memory>
#include <string>

#include <User.pb.h>

namespace SimpleChat {

class User;
class Chatroom;
class ChatConnection;
class ChatTarget;
class AbstractMessage;

class Chatee
{
public:
    explicit Chatee(const User& user,
                    ChatConnection* connection, 
                    std::shared_ptr<Chatroom> chatroom);

    virtual ~Chatee();

    User& user();

    bool sendMessage(std::unique_ptr<AbstractMessage> message);

    void sendChatMessage(const std::string& message, const std::string& from, const std::string& target = "");
    void sendResponse(bool success, const std::string& message);

    void mute(bool propagate);
    void unmute(bool propagate);
    void kick(bool propagate);

    ChatConnection* connection() const;

    bool authorized() const;
    void setAuthorized(bool authorized);

private:
    User user_;
    ChatConnection* connection_;
    std::weak_ptr<Chatroom> chatroom_;
    bool authorized_;

    void propagateChange(int type);

    std::unique_ptr<ChatTarget> getSelf();
    std::unique_ptr<ChatTarget> getTarget(const std::string& target);

    template<typename MessageType>
    void prepareAndSend(std::unique_ptr<MessageType> message);
};

} // SimpleChat namespace