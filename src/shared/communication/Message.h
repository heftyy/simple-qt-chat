#pragma once

#include <memory>
#include <type_traits>

#include "User.pb.h"
#include "AbstractMessage.h"
#include "NetworkMessage.pb.h"

namespace SimpleChat {

template
<
	typename MessageType,
	typename /* dummy */ = typename std::enable_if<
		std::is_base_of<ProtobufMsg, MessageType>::value
	>::type
>
class Message : public AbstractMessage {
public:
	explicit Message(std::unique_ptr<MessageType> message, int type)
		: message_(std::move(message)),
		type_(type) {

	}

	std::string serialize() override {
		std::string serializeMessage;
		if (message_->IsInitialized()) {
			return message_->SerializeAsString();
		}

		return nullptr;
	}

	virtual int type() override {
		return type_;
	};

	virtual std::unique_ptr<ProtobufMsg> message() override {
		return std::move(message_);
	}

	virtual bool isInitialized() override {
		return message_ != nullptr && 
			message_->IsInitialized() &&
			NetworkMessageType_IsValid(type_);
	}

private:
	std::unique_ptr<MessageType> message_;
	int type_;
};

}

