#include "Chatroom.h"
#include "Chatee.h"
#include "ChatConnection.h"
#include "../communication/Message.h"

#include "User.pb.h"
#include "ChatMessage.pb.h"
#include "NetworkMessage.pb.h"

namespace SimpleChat {

Chatroom::Chatroom() : 
    nextUserId(1) {
}

std::tuple<bool, std::string, std::shared_ptr<Chatee>>
Chatroom::chateeJoined(const std::string& name,
                       ChatConnection* connection) {

    if (chateeExists(name))
        return std::make_tuple(false, "That name is already taken", nullptr);

    User user;
    user.set_id(nextUserId++);
    user.set_name(name);
    user.set_presence(ONLINE);

    return chateeJoined(user, connection);
}

std::tuple<bool, std::string, std::shared_ptr<Chatee>> Chatroom::chateeJoined(const User& user,
                                                                              ChatConnection* connection) {
    if (chateeExists(user.name()))
        return std::make_tuple(false, "That name is already taken", nullptr);

    auto chatee = std::make_shared<Chatee>(user, connection, shared_from_this());
    auto success = insertChatee(chatee);

    if (success) {
        if(connection != nullptr)
            connection->setChatee(chatee);
        return std::make_tuple(true, "", chatee);
    }

    return std::make_tuple(false, "Failed to add chatee.", nullptr);
}

std::tuple<bool, std::string, std::shared_ptr<Chatee>> Chatroom::chateeJoined(const std::shared_ptr<Chatee>& chatee) {
    if (chateeExists(chatee->user().name()))
        return std::make_tuple(false, "That name is already taken", nullptr);

    auto success = insertChatee(chatee);
    if (success) {
        return std::make_tuple(true, "", chatee);
    }

    return std::make_tuple(false, "Failed to add chatee.", nullptr);
}

std::tuple<bool, std::string>
Chatroom::chateeLeft(const std::string& name) {
    if (!chateeExists(name)) {
        return std::make_tuple(false, "Chatee doesn't exist.");
    }

    auto success = removeChatee(name);
    if (success)
        return std::make_tuple(true, "");

    return std::make_tuple(false, "Failed to remove chatee.");
}

std::tuple<bool, std::string>
Chatroom::chateeLeft(const User& user) {
    return chateeLeft(user.name());
}

void Chatroom::propagateMessage(std::unique_ptr<AbstractMessage> abstractMessage) const {
    for (auto& pair : chatees_) {
        pair.second->sendMessage(abstractMessage->clone());
    }
}

bool Chatroom::chateeExists(const std::string& name) {
    std::lock_guard<std::mutex> guard(this->chatees_mutex_);

    auto search = chatees_.find(name);
    return search != chatees_.end();
}

std::shared_ptr<Chatee> Chatroom::getChatee(const std::string& name) {
    if (chateeExists(name)) {
        return chatees_[name];
    }

    return nullptr;
}

std::unique_ptr<ChatTarget> Chatroom::getTarget(const std::string& userName) {
    std::lock_guard<std::mutex> guard(this->chatees_mutex_);

    auto search = chatees_.find(userName);
    if (search != chatees_.end()) {
        auto chatTarget = std::make_unique<ChatTarget>();
        chatTarget->set_user_id(search->second->user().id());
        chatTarget->set_user_name(search->second->user().name());

        return std::move(chatTarget);
    }

    return nullptr;
}

void Chatroom::setMotd(const std::string& motd) {
    motd_ = motd;

    auto chatroomChanged = std::make_unique<ChatroomChange>();
    chatroomChanged->set_motd(motd);

    propagateMessage(MessageBuilder::build(std::move(chatroomChanged)));
}

std::string Chatroom::motd() {
    return motd_;
}

const ChateesMap& Chatroom::map() const {
    return chatees_;
}

bool Chatroom::insertChatee(const std::shared_ptr<Chatee>& chatee) {
    if (!chateeExists(chatee->user().name())) {
        std::lock_guard<std::mutex> guard(this->chatees_mutex_);
        auto result = chatees_.insert(std::make_pair(chatee->user().name(), std::move(chatee)));

        return result.first->second != nullptr;
    }

    return false;
}

bool Chatroom::removeChatee(const std::string& userName) {
    if (chateeExists(userName)) {
        std::lock_guard<std::mutex> guard(this->chatees_mutex_);
        chatees_.erase(userName);

        return true;
    }

    return false;
}

} // SimpleChat namespace
