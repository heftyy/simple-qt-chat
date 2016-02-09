#pragma once

#include <map>
#include <memory>
#include <mutex>
#include <communication/AbstractMessage.h>

namespace SimpleChat {

class User;
class Chatee;
class ChatConnection;
class AbstractMessage;
class ChatTarget;
class ChatMessage;
class ChatCommand;
class ChatAuthorize;

using ChateesMap = std::map<std::string, std::shared_ptr<Chatee>>;

class Chatroom
{
public:
    explicit Chatroom();
    ~Chatroom();

    std::tuple<bool, std::string, std::shared_ptr<Chatee>>
        chateeJoined(const std::string& name,
                     const std::shared_ptr<ChatConnection>& connection);

    std::tuple<bool, std::string>
        chateeLeft(const std::string& name);

    bool sendMessage(const std::string& name,
                     std::unique_ptr<AbstractMessage> message);

    void propagateMessage(std::unique_ptr<AbstractMessage> abstractMessage) const;

    bool chateeExists(const std::string& name);

    std::shared_ptr<Chatee> getChatee(const std::string& name);
    std::unique_ptr<ChatTarget> getTarget(const std::string& userName);

private:
    int nextUserId;

    std::mutex chatees_mutex_;
    ChateesMap chatees_;
    std::string motd_;

    bool insertChatee(std::shared_ptr<Chatee> chatee);
    bool removeChatee(const std::string& userName);
};

} // SimpleChat namespace