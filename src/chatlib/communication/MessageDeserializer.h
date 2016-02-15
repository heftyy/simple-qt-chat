#pragma once

#include <memory>
#include <string>

#include "NetworkMessage.pb.h"

namespace SimpleChat {

class AbstractMessage;

/*!
 * MessageDeserializer holds a serialized message string and
 * creates the NetworkMessage instance for easy NetworkMessageType access.
 */
class MessageDeserializer {
public:
    explicit MessageDeserializer(const std::string& serializedMessage);

    /*!
     *
     */
    template<typename MessageType>
    auto getMessage() const -> std::unique_ptr<MessageType>;

    int type() const;
    bool isInitialized() const;

private:
    std::string serializedMessage_;
    NetworkMessage networkMessage_;
};

} // SimpleChat namespace
