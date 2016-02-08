#include "MessageDeserializer.h"
#include "AbstractMessage.h"
#include "NetworkMessage.pb.h"
#include "User.pb.h"
#include "ChatMessage.pb.h"

namespace SimpleChat {

MessageDeserializer::MessageDeserializer(const std::string& serializedMessage)
    : serializedMessage_(serializedMessage),
    networkMessage_(getNetworkMessage()) {

}

template<typename MessageType>
auto MessageDeserializer::getMessage() const -> std::unique_ptr<MessageType> {
    auto message = std::make_unique<MessageType>();
    message->ParseFromString(networkMessage_->serialized_data());
    return std::move(message);
}

template auto MessageDeserializer::getMessage<UserJoinRequest>() const->std::unique_ptr<UserJoinRequest>;
template auto MessageDeserializer::getMessage<UserJoinResponse>() const->std::unique_ptr<UserJoinResponse>;

std::unique_ptr<NetworkMessage> MessageDeserializer::getNetworkMessage() const {
    auto networkMessage = std::make_unique<NetworkMessage>();
    networkMessage->ParseFromString(serializedMessage_);

    return std::move(networkMessage);
}

int MessageDeserializer::type() const {
    return networkMessage_->header().type();
}

bool MessageDeserializer::isInitialized() const {
    return networkMessage_ != nullptr &&
        networkMessage_->IsInitialized() &&
        NetworkMessageType_IsValid(type());
}

} // SimpleChat namespace
