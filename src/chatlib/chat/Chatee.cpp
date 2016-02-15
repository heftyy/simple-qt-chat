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
               ChatConnection* connection, 
               std::shared_ptr<Chatroom> chatroom) :
        user_(user),
        connection_(connection), 
        chatroom_(chatroom),
        authorized_(false) {

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
            std::move(chatMessage)
    );
}

void Chatee::sendResponse(bool success, const std::string& message) {
    auto genericResponse = std::make_unique<GenericChatResponse>();

    genericResponse->set_success(success);
    genericResponse->set_message(message);

    this->prepareAndSend(
            std::move(genericResponse)
    );
}

void Chatee::mute(bool propagate) {
    if(propagate) {
        propagateChange(MUTED);
    }

    user_.set_mute(true);
}

void Chatee::unmute(bool propagate) {
    if(propagate) {
        propagateChange(UNMUTED);
    }

    user_.set_mute(false);
}

void Chatee::kick(bool propagate) {   
    if(propagate) {
        propagateChange(KICKED);

        /*! propagate will be true when the server calls this function
         * so we should drop the connection to the kicked client */
        connection_->disconnectFromHost();
    }

    chatroom_.lock()->chateeLeft(user_.name());
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

void Chatee::propagateChange(int type) {
    auto response = std::make_unique<UserChange>();
    response->set_action(static_cast<UserAction>(type));
    response->mutable_user()->CopyFrom(user());

    chatroom_.lock()->propagateMessage(
            MessageBuilder::build(std::move(response)));
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
void Chatee::prepareAndSend(std::unique_ptr<MessageType> message) {
    sendMessage(MessageBuilder::build(std::move(message)));
}

template void Chatee::prepareAndSend<UserJoinRequest>(std::unique_ptr<UserJoinRequest> message);
template void Chatee::prepareAndSend<UserJoinResponse>(std::unique_ptr<UserJoinResponse> message);

template void Chatee::prepareAndSend<UserListRequest>(std::unique_ptr<UserListRequest> message);
template void Chatee::prepareAndSend<UserListResponse>(std::unique_ptr<UserListResponse> message);

template void Chatee::prepareAndSend<UserChange>(std::unique_ptr<UserChange> message);

template void Chatee::prepareAndSend<ChatMessage>(std::unique_ptr<ChatMessage> message);
template void Chatee::prepareAndSend<ChatCommand>(std::unique_ptr<ChatCommand> message);

template void Chatee::prepareAndSend<ChatroomChange>(std::unique_ptr<ChatroomChange> message);

template void Chatee::prepareAndSend<GenericChatResponse>(std::unique_ptr<GenericChatResponse> message);

} // SimpleChat namespace