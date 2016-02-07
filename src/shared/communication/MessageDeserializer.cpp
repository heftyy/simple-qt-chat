#include "MessageDeserializer.h"
#include "Message.h"
#include "NetworkMessage.pb.h"
#include "ChatMessage.pb.h"
#include "User.pb.h"

namespace SimpleChat {

MessageDeserializer::MessageDeserializer(const std::string& serializedMessage)
	: serializedMessage_(serializedMessage) {

}

std::unique_ptr<Message> MessageDeserializer::getMessage() {
	auto networkMessage = getNetworkMessage();

	std::unique_ptr<protoMsg> protoMsg;

	switch (networkMessage->header().type()) {
	case USER_JOIN_REQUEST:
		protoMsg = std::make_unique<UserJoinRequest>();
		break;
	case USER_JOIN_RESPONSE:
		protoMsg = std::make_unique<UserJoinResponse>();
		break;
	case USER_LIST_REQUEST:
		protoMsg = std::make_unique<UserListRequest>();
		break;
	case USER_LIST_RESPONSE:
		protoMsg = std::make_unique<UserListResponse>();
		break;
	case USER_STATUS_CHANGE:
		protoMsg = std::make_unique<UserStatusChange>();
		break;
	case USER_LIST_CHANGE:
		protoMsg = std::make_unique<UserListChange>();
		break;
	case CHAT_MESSAGE:
		protoMsg = std::make_unique<ChatMessage>();
		break;
	case CHAT_AUTHORIZE:
		protoMsg = std::make_unique<ChatAuthorize>();
		break;
	case CHAT_COMMAND:
		protoMsg = std::make_unique<ChatCommand>();
		break;
	default: break;
	}

	if (!protoMsg) return nullptr;

	protoMsg->ParseFromString(networkMessage->serialized_data());
	auto message = std::make_unique<Message>(std::move(protoMsg));

	return std::move(message);
}

std::unique_ptr<NetworkMessage> MessageDeserializer::getNetworkMessage() const {
	auto networkMessage = std::make_unique<NetworkMessage>();
	networkMessage->ParseFromString(serializedMessage_);

	return std::move(networkMessage);
}

}
