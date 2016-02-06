#include "Message.h"

namespace SimpleChat {

Message::Message(std::unique_ptr<protoMsg> message)
	: message_(std::move(message)) {
	
}

std::string Message::serialize() {
	std::string serializeMessage;
	if(message_->IsInitialized()) {
		message_->SerializeToString(&serializeMessage);
		return serializeMessage;
	}
		
	return nullptr;
}

}