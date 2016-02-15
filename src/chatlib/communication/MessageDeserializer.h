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
     * Uses the given template to create a message and fills the data.
     * You must make sure that the message type matches the template
     * before calling this method.
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
