#include "Chatroom.h"
#include "Chatee.h"
#include "communication/AbstractMessage.h"
#include "proto/User.pb.h"

namespace SimpleChat {

Chatroom::Chatroom() {
}


Chatroom::~Chatroom() {
}

void Chatroom::chateeJoined(std::unique_ptr<User> user) {
	auto userId = user->id();
	auto chatee = std::unique_ptr<Chatee>(new Chatee(std::move(user)));
	
	insertChatee(std::move(chatee));
}

void Chatroom::chateeLeft(std::unique_ptr<User> user) {
	removeChatee(user->id());
}

void Chatroom::newMessage(std::unique_ptr<AbstractMessage> abstractMessage) {
	
}

void Chatroom::insertChatee(std::unique_ptr<Chatee> chatee) {
	std::lock_guard<std::mutex> guard(this->chatees_mutex_);

	auto search = chatees_.find(chatee->getUserId());
	if(search != chatees_.end()) {
		chatees_.insert(std::make_pair(chatee->getUserId(), std::move(chatee)));
	}	
}

void Chatroom::removeChatee(int userId) {
	std::lock_guard<std::mutex> guard(this->chatees_mutex_);
	auto search = chatees_.find(userId);
	if (search != chatees_.end()) {
		chatees_.erase(search);
	}
}

}
