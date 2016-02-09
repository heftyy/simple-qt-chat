#include "MessageSerializer.h"
#include "NetworkMessage.pb.h"
#include "AbstractMessage.h"

namespace SimpleChat {

MessageSerializer::MessageSerializer(std::unique_ptr<AbstractMessage> abstractMessage,
                                     const std::string& dest,
                                     const std::string& src) : 
    abstractMessage_(std::move(abstractMessage)),
    dest_(dest),
    src_(src) {

}


std::tuple<bool, std::string> MessageSerializer::serialize() const {
    if (!abstractMessage_->isInitialized())
        return std::make_tuple(false, "");

    NetworkMessage networkMessage;
    auto networkMessageHeader = networkMessage.mutable_header();
    networkMessageHeader->set_src(src_);
    networkMessageHeader->set_dest(dest_);
    networkMessageHeader->set_type(
        static_cast<NetworkMessageType>(abstractMessage_->type())
    );

    networkMessage.set_serialized_data(abstractMessage_->serialize());

    return std::make_tuple(true, networkMessage.SerializeAsString());
}

} // SimpleChat namespace