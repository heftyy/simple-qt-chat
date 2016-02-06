#pragma once

#include <memory>

#include "proto/ChatMessage.pb.h"
#include "AbstractMessage.h"

namespace SimpleChat {

class Message : public AbstractMessage {
    using protoMsg = google::protobuf::Message;

public:
	explicit Message(std::unique_ptr<protoMsg> message);

	std::string serialize() override;

	std::unique_ptr<protoMsg> message_;
};

}

