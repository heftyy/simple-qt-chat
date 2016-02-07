#pragma once

#include <memory>

#include "proto/ChatMessage.pb.h"
#include "AbstractMessage.h"

namespace SimpleChat {

using protoMsg = google::protobuf::Message;

class Message : public AbstractMessage {
public:
	explicit Message(std::unique_ptr<protoMsg> message);

	std::string serialize() override;

	void setMessage(std::unique_ptr<protoMsg> protoMsg);

private:
	std::unique_ptr<protoMsg> message_;
};

}

