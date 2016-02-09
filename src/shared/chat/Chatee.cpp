#include <chrono>
#include <cctype>

#include "../communication/Message.h"
#include "Chatee.h"
#include "Chatroom.h"
#include "ChatConnection.h"
#include "commands/CommandParser.h"

#include "ChatMessage.pb.h"
#include "User.pb.h"
#include "NetworkMessage.pb.h"

namespace SimpleChat {

Chatee::Chatee(std::unique_ptr<User> user,
               const std::shared_ptr<ChatConnection>& connection) :
        user_(std::move(user)),
        connection_(connection) {

}

Chatee::~Chatee() {
}

bool Chatee::sendMessage(std::unique_ptr<AbstractMessage> message) {
    if (connection_) {
        return connection_->sendMessage(std::move(message));
    }

    return false;
}

void Chatee::sendMessage(const std::string& message, const std::string& target) {
    auto chatMessage = std::make_unique<ChatMessage>();
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    chatMessage->set_text(message);
    chatMessage->set_allocated_from(this->getSelf().release());
    chatMessage->set_timestamp(millis);

    if (!target.empty())
        chatMessage->set_allocated_target(this->getTarget(target).release());

    this->prepareAndSend(
            std::move(chatMessage),
            CHAT_MESSAGE
    );
}

void Chatee::sendCommand(const std::string& command) {
    auto commandParser = std::make_unique<CommandParser>(command);
    auto chatCommand = commandParser->chatCommand(
        std::move(getSelf())
    );

    this->prepareAndSend(
            std::move(chatCommand),
            CHAT_COMMAND
    );
}

void Chatee::authorize(const std::string& password) {
    auto authorizeMessage = std::make_unique<ChatAuthorize>();
    authorizeMessage->set_allocated_from(getSelf().release());
    authorizeMessage->set_password(password);

    this->prepareAndSend(
            std::move(authorizeMessage),
            CHAT_AUTHORIZE
    );
}

std::unique_ptr<ChatTarget> Chatee::getSelf() {
    if (!chatroom_.expired()) {
        return std::move(chatroom_.lock()->getTarget(user_->name()));
    }

    return nullptr;
}

std::unique_ptr<ChatTarget> Chatee::getTarget(const std::string& target) {
    if (!chatroom_.expired()) {
        return std::move(chatroom_.lock()->getTarget(target));
    }

    return nullptr;
}

User& Chatee::user() {
    return *user_;
}

template<typename MessageType>
void Chatee::prepareAndSend(std::unique_ptr<MessageType> message, int type) {
    auto abstractMessage = std::make_unique<Message<MessageType>>(
            std::move(message),
            type
    );
    sendMessage(std::move(abstractMessage));
}

template void Chatee::prepareAndSend<UserJoinRequest>(std::unique_ptr<UserJoinRequest> message, int type);
template void Chatee::prepareAndSend<UserJoinResponse>(std::unique_ptr<UserJoinResponse> message, int type);

template void Chatee::prepareAndSend<UserListRequest>(std::unique_ptr<UserListRequest> message, int type);
template void Chatee::prepareAndSend<UserListResponse>(std::unique_ptr<UserListResponse> message, int type);

template void Chatee::prepareAndSend<UserStatusChange>(std::unique_ptr<UserStatusChange> message, int type);
template void Chatee::prepareAndSend<UserListChange>(std::unique_ptr<UserListChange> message, int type);

template void Chatee::prepareAndSend<ChatMessage>(std::unique_ptr<ChatMessage> message, int type);
template void Chatee::prepareAndSend<ChatAuthorize>(std::unique_ptr<ChatAuthorize> message, int type);
template void Chatee::prepareAndSend<ChatCommand>(std::unique_ptr<ChatCommand> message, int type);

} // SimpleChat namespace