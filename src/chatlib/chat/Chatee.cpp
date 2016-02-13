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

Chatee::Chatee(const User& user,
               ChatConnection* connection) :
        user_(user),
        connection_(connection) {

}

Chatee::~Chatee() {
}

bool Chatee::sendMessage(std::unique_ptr<AbstractMessage> message) {
    if (connection_ != nullptr) {
        return connection_->sendMessage(std::move(message));
    }

    return false;
}

void Chatee::sendChatMessage(const std::string& message, const std::string& from, const std::string& target) {
    auto chatMessage = std::make_unique<ChatMessage>();
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    chatMessage->set_text(message);
    if(target.empty())
        chatMessage->set_allocated_target(getSelf().release());
    else
        chatMessage->set_allocated_target(getTarget(target).release());

    chatMessage->set_allocated_from(getTarget(from).release());

    this->prepareAndSend(
            std::move(chatMessage),
            CHAT_MESSAGE
    );
}

/*
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
*/

void Chatee::sendResponse(bool success, const std::string& message) {
    auto genericResponse = std::make_unique<GenericChatResponse>();

    genericResponse->set_success(success);
    genericResponse->set_message(message);

    this->prepareAndSend(
            std::move(genericResponse),
            GENERIC_CHAT_RESPONSE
    );
}

void Chatee::mute(bool propagate) {
    user_.set_status(MUTED);

    if(!propagate)
        return;

    auto response = std::make_unique<UserChange>();
    response->set_status(MUTED);
    response->mutable_user()->CopyFrom(user());

    chatroom_.lock()->propagateMessage(std::make_unique<Message<UserChange>>(
            std::move(response), USER_CHANGE));
}

void Chatee::kick(bool propagate) {
    chatroom_.lock()->chateeLeft(user_.name());

    if(!propagate)
        return;

    auto response = std::make_unique<UserChange>();
    response->set_action(KICKED);
    response->mutable_user()->CopyFrom(user());

    chatroom_.lock()->propagateMessage(std::make_unique<Message<UserChange>>(
            std::move(response), USER_CHANGE));
}

ChatConnection* Chatee::connection() const {
    return connection_;
}

bool Chatee::authorized() const {
    return authorized_;
}

void Chatee::setAuthorized(bool authorized) {
    authorized_ = authorized;
}

std::unique_ptr<ChatTarget> Chatee::getSelf() {
    if (!chatroom_.expired()) {
        return std::move(chatroom_.lock()->getTarget(user_.name()));
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
    return user_;
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

template void Chatee::prepareAndSend<UserChange>(std::unique_ptr<UserChange> message, int type);

template void Chatee::prepareAndSend<ChatMessage>(std::unique_ptr<ChatMessage> message, int type);
template void Chatee::prepareAndSend<ChatAuthorize>(std::unique_ptr<ChatAuthorize> message, int type);
template void Chatee::prepareAndSend<ChatCommand>(std::unique_ptr<ChatCommand> message, int type);

template void Chatee::prepareAndSend<ChatroomChange>(std::unique_ptr<ChatroomChange> message, int type);

template void Chatee::prepareAndSend<GenericChatResponse>(std::unique_ptr<GenericChatResponse> message, int type);

} // SimpleChat namespace