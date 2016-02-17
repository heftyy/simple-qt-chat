#pragma once

#include <map>
#include <memory>
#include <mutex>

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

class Chatroom : public std::enable_shared_from_this<Chatroom>
{
public:
    explicit Chatroom();

    /*!
     * Uses the name and ChatConnection to create a User and Chatee instances.
     * Chatee contains the User.
     * returns:
     *   - bool success
     *   - std::string error message
     *   - shared_ptr Chatee
     */
    std::tuple<bool, std::string, std::shared_ptr<Chatee>>
    chateeJoined(const std::string& name,
                 ChatConnection* connection);

    /*!
     * Uses the User and ChatConnection to create Chatee instance.
     */
    std::tuple<bool, std::string, std::shared_ptr<Chatee>>
    chateeJoined(const User& user,
                 ChatConnection* connection);

    /*!
     * Checks if the chatee already exists and adds it if not.
     */
    std::tuple<bool, std::string, std::shared_ptr<Chatee>>
    chateeJoined(const std::shared_ptr<Chatee>& chatee);

    /*!
     * Removes the chatee with the given name from map.
     */
    std::tuple<bool, std::string>
    chateeLeft(const std::string& name);

    std::tuple<bool, std::string>
    chateeLeft(const User& user);

    /*!
     * Clones the message and sends it to all chatees
     */
    void propagateMessage(std::unique_ptr<AbstractMessage> abstractMessage) const;

    bool chateeExists(const std::string& name);

    std::shared_ptr<Chatee> getChatee(const std::string& name);
    std::unique_ptr<ChatTarget> getTarget(const std::string& userName);

    void setMotd(const std::string& motd);
    std::string motd();

    const ChateesMap& map() const;

private:
    int nextUserId;

    std::mutex chatees_mutex_;
    ChateesMap chatees_;
    std::string motd_;

    bool insertChatee(const std::shared_ptr<Chatee>& chatee);
    bool removeChatee(const std::string& userName);
};

} // SimpleChat namespace