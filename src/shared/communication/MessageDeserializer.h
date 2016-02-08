#pragma once

#include <memory>
#include <string>

namespace SimpleChat {

class NetworkMessage;
class AbstractMessage;

class MessageDeserializer {
public:
    explicit MessageDeserializer(const std::string& serializedMessage);

    template<typename MessageType>
    auto getMessage() const->std::unique_ptr<MessageType>;

    int type() const;
    bool isInitialized() const;

private:
    std::string serializedMessage_;
    std::unique_ptr<NetworkMessage> networkMessage_;

    std::unique_ptr<NetworkMessage> getNetworkMessage() const;
};

} // SimpleChat namespace
