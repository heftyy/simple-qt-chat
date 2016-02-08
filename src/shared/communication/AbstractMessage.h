#pragma once

#include <string>
#include <memory>

#include <google/protobuf/message.h>

namespace SimpleChat {

using ProtobufMsg = google::protobuf::Message;

class AbstractMessage {
public:
    virtual std::string serialize() = 0;

    virtual int type() = 0;
    virtual std::unique_ptr<ProtobufMsg> message() = 0;
    virtual bool isInitialized() = 0;
	virtual ~AbstractMessage() {};
};

}

