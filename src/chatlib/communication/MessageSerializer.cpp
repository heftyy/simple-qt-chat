#include "MessageSerializer.h"
#include "NetworkMessage.pb.h"
#include "AbstractMessage.h"

namespace SimpleChat {

MessageSerializer::MessageSerializer(std::unique_ptr<AbstractMessage> abstractMessage) :
    abstractMessage_(std::move(abstractMessage)) {

}


std::tuple<bool, std::string> MessageSerializer::serialize() const {
    if (abstractMessage_ == nullptr || !abstractMessage_->isInitialized())
        return std::make_tuple(false, "");

    NetworkMessage networkMessage;
    networkMessage.set_serialized_data(abstractMessage_->serialize());

    auto networkMessageHeader = networkMessage.mutable_header();
    networkMessageHeader->set_type(
        static_cast<NetworkMessageType>(abstractMessage_->type())
    );
    networkMessageHeader->set_size(1);

    return std::make_tuple(true, networkMessage.SerializeAsString());
}

} // SimpleChat namespace