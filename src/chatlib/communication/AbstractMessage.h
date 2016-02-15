#pragma once

#include <string>
#include <memory>

#include <google/protobuf/message.h>

namespace SimpleChat {

using ProtobufMsg = google::protobuf::Message;

/*!
 * Interface for all protobuf messages.
 */
class AbstractMessage {
public:
    virtual ~AbstractMessage() {};

    virtual std::string serialize() = 0;

    virtual int type() = 0;
    virtual bool isInitialized() = 0;    

    virtual std::unique_ptr<AbstractMessage> clone() = 0;
};

} // SimpleChat namespace

