#include "MessageDeserializer.h"
#include "AbstractMessage.h"
#include "NetworkMessage.pb.h"
#include "User.pb.h"
#include "ChatMessage.pb.h"

namespace SimpleChat {

MessageDeserializer::MessageDeserializer(const std::string& serializedMessage) : 
    serializedMessage_(serializedMessage),
    networkMessage_(getNetworkMessage()) {

}

template<typename MessageType>
auto MessageDeserializer::getMessage() const -> std::unique_ptr<MessageType> {
    if(!networkMessage_->IsInitialized())
        return nullptr;

    auto message = std::make_unique<MessageType>();
    message->ParseFromString(networkMessage_->serialized_data());
    return std::move(message);
}

template auto MessageDeserializer::getMessage<UserJoinRequest>() const -> std::unique_ptr<UserJoinRequest>;
template auto MessageDeserializer::getMessage<UserJoinResponse>() const -> std::unique_ptr<UserJoinResponse>;

template auto MessageDeserializer::getMessage<UserListRequest>() const -> std::unique_ptr<UserListRequest>;
template auto MessageDeserializer::getMessage<UserListResponse>() const -> std::unique_ptr<UserListResponse>;

template auto MessageDeserializer::getMessage<UserChange>() const -> std::unique_ptr<UserChange>;

template auto MessageDeserializer::getMessage<ChatMessage>() const -> std::unique_ptr<ChatMessage>;
template auto MessageDeserializer::getMessage<ChatAuthorize>() const -> std::unique_ptr<ChatAuthorize>;
template auto MessageDeserializer::getMessage<ChatCommand>() const -> std::unique_ptr<ChatCommand>;

template auto MessageDeserializer::getMessage<GenericChatResponse>() const -> std::unique_ptr<GenericChatResponse>;

std::unique_ptr<NetworkMessage> MessageDeserializer::getNetworkMessage() const {
    auto networkMessage = std::make_unique<NetworkMessage>();
    networkMessage->ParseFromString(serializedMessage_);

    return std::move(networkMessage);
}

int MessageDeserializer::type() const {
    if(networkMessage_->IsInitialized())
        return networkMessage_->header().type();
    return -1;
}

bool MessageDeserializer::isInitialized() const {
    return networkMessage_ != nullptr &&
        networkMessage_->IsInitialized() &&
        NetworkMessageType_IsValid(type());
}

} // SimpleChat namespace
