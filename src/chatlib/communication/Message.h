#pragma once

#include <memory>
#include <type_traits>

#include <ChatMessage.pb.h>
#include <NetworkMessage.pb.h>
#include <User.pb.h>

#include "AbstractMessage.h"

namespace SimpleChat {

template
<
    typename MessageType,
    typename /* dummy */ = typename std::enable_if<
    std::is_base_of<ProtobufMsg, MessageType>::value
    >::type
>
class Message : public AbstractMessage {
public:
    explicit Message(std::unique_ptr<MessageType> message, int type) :
            message_(std::move(message)),
            type_(type) {

    }

    std::string serialize() override {
        std::string serializeMessage;
        if (message_->IsInitialized()) {
            return message_->SerializeAsString();
        }

        return nullptr;
    }

    virtual int type() override {
        return type_;
    };

    virtual bool isInitialized() override {
        return message_ != nullptr &&
            message_->IsInitialized() &&
            NetworkMessageType_IsValid(type_);
    }

    virtual std::unique_ptr<AbstractMessage> clone() override {
        auto msg = std::make_unique<MessageType>(*message_);
        return std::make_unique<Message<MessageType>>(std::move(msg), type_);
    }

private:
    std::unique_ptr<MessageType> message_;
    int type_;
};

class MessageBuilder {
public:
    static std::unique_ptr<Message<UserJoinRequest>> build(std::unique_ptr<UserJoinRequest> message) {
        return std::make_unique<Message<UserJoinRequest>>(std::move(message), USER_JOIN_REQUEST);
    }

    static std::unique_ptr<Message<UserJoinResponse>> build(std::unique_ptr<UserJoinResponse> message) {
        return std::make_unique<Message<UserJoinResponse>>(std::move(message), USER_JOIN_RESPONSE);
    }

    static std::unique_ptr<Message<UserListRequest>> build(std::unique_ptr<UserListRequest> message) {
        return std::make_unique<Message<UserListRequest>>(std::move(message), USER_LIST_REQUEST);
    }

    static std::unique_ptr<Message<UserListResponse>> build(std::unique_ptr<UserListResponse> message) {
        return std::make_unique<Message<UserListResponse>>(std::move(message), USER_LIST_RESPONSE);
    }

    static std::unique_ptr<Message<UserChange>> build(std::unique_ptr<UserChange> message) {
        return std::make_unique<Message<UserChange>>(std::move(message), USER_CHANGE);
    }

    static std::unique_ptr<Message<ChatMessage>> build(std::unique_ptr<ChatMessage> message) {
        return std::make_unique<Message<ChatMessage>>(std::move(message), CHAT_MESSAGE);
    }

    static std::unique_ptr<Message<ChatCommand>> build(std::unique_ptr<ChatCommand> message) {
        return std::make_unique<Message<ChatCommand>>(std::move(message), CHAT_COMMAND);
    }

    static std::unique_ptr<Message<ChatroomChange>> build(std::unique_ptr<ChatroomChange> message) {
        return std::make_unique<Message<ChatroomChange>>(std::move(message), CHATROOM_CHANGE);
    }

    static std::unique_ptr<Message<GenericChatResponse>> build(std::unique_ptr<GenericChatResponse> message) {
        return std::make_unique<Message<GenericChatResponse>>(std::move(message), GENERIC_CHAT_RESPONSE);
    }
};

} // SimpleChat namespace

