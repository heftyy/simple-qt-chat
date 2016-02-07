#include "Chatroom.h"
#include "Chatee.h"
#include "communication/AbstractMessage.h"
#include "communication/Message.h"
#include "User.pb.h"
#include "ChatMessage.pb.h"

namespace SimpleChat {

Chatroom::Chatroom() {
}


Chatroom::~Chatroom() {
}

void Chatroom::chateeJoined(std::unique_ptr<User> user) {
	auto userId = user->id();
	auto chatee = std::make_unique<Chatee>(std::move(user));
	
	insertChatee(std::move(chatee));
}

void Chatroom::chateeLeft(std::unique_ptr<User> user) {
	removeChatee(user->name());
}

void Chatroom::messageReceived(std::unique_ptr<ChatMessage> chatMessage) {
	auto msg = std::make_unique<Message>(std::move(chatMessage));
	propageMessage(std::move(msg));
}

void Chatroom::commandReceived(std::unique_ptr<ChatCommand> chatCommand) {
	auto msg = std::make_unique<Message>(std::move(chatCommand));
	propageMessage(std::move(msg));
}

void Chatroom::authorizeReceived(std::unique_ptr<ChatAuthorize> chatAuthorize) {
	auto msg = std::make_unique<Message>(std::move(chatAuthorize));
	propageMessage(std::move(msg));
}

std::unique_ptr<ChatTarget> Chatroom::getTarget(const std::string& userName) {
	std::lock_guard<std::mutex> guard(this->chatees_mutex_);

	auto search = chatees_.find(userName);
	if (search != chatees_.end()) {
		auto chatTarget = std::make_unique<ChatTarget>();
		chatTarget->set_user_id(search->second->getUserId());
		chatTarget->set_user_name(search->second->getUserName());

		return std::move(chatTarget);
	}

	return nullptr;
}

void Chatroom::insertChatee(std::unique_ptr<Chatee> chatee) {
	std::lock_guard<std::mutex> guard(this->chatees_mutex_);

	auto search = chatees_.find(chatee->getUserName());
	if(search != chatees_.end()) {
		chatees_.insert(std::make_pair(chatee->getUserName(), std::move(chatee)));
	}	
}

void Chatroom::removeChatee(const std::string& userName) {
	std::lock_guard<std::mutex> guard(this->chatees_mutex_);

	auto search = chatees_.find(userName);
	if (search != chatees_.end()) {
		chatees_.erase(search);
	}
}

void Chatroom::propageMessage(std::unique_ptr<AbstractMessage> abstractMessage) {
	for (auto& pair : chatees_) {
		pair.second->sendMessage(std::move(abstractMessage));
	}
}

}
