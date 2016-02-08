#include "Chatroom.h"
#include "Chatee.h"
#include "ChatConnection.h"
#include "User.pb.h"
#include "ChatMessage.pb.h"

namespace SimpleChat {

Chatroom::Chatroom()
    : nextUserId(0) {
}


Chatroom::~Chatroom() {
}

std::tuple<bool, std::string, std::shared_ptr<Chatee>>
Chatroom::chateeJoined(const std::string& name,
                       const std::shared_ptr<ChatConnection>& connection) {

    if (chateeExists(name))
        return std::make_tuple(false, "That name is already taken", nullptr);

    auto user = std::make_unique<User>();
    user->set_id(nextUserId++);
    user->set_name(name);
    user->set_status(ONLINE);

    auto chatee = std::make_shared<Chatee>(std::move(user), connection);
    auto success = insertChatee(chatee);

    if (success)
        return std::make_tuple(true, "", chatee);

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

bool Chatroom::sendMessage(const std::string& name, std::unique_ptr<AbstractMessage> message) {
    if (chateeExists(name)) {
        return chatees_[name]->sendMessage(std::move(message));
    }

    return false;
}

void Chatroom::incomingMessage(std::unique_ptr<ChatMessage> chatMessage) {
    //	auto msg = MessageFactory::Create(std::move(chatMessage));
    //    propagateMessage(std::move(msg));
}

void Chatroom::incomingMessage(std::unique_ptr<ChatCommand> chatCommand) {
    //	auto msg = MessageFactory::Create(std::move(chatCommand));
    //    propagateMessage(std::move(msg));
}

void Chatroom::incomingMessage(std::unique_ptr<ChatAuthorize> chatAuthorize) {
    //	auto msg = MessageFactory::Create(std::move(chatAuthorize));
    //    propagateMessage(std::move(msg));
}

void Chatroom::propagateMessage(std::unique_ptr<AbstractMessage> abstractMessage) const {
    for (auto& pair : chatees_) {
        pair.second->sendMessage(std::move(abstractMessage));
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

bool Chatroom::insertChatee(std::shared_ptr<Chatee> chatee) {
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
