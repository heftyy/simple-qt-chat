#include <chrono>
#include <cctype>

#include "../Message.h"
#include "Chatee.h"
#include "Chatroom.h"
#include "CommandParser.h"

#include "ChatMessage.pb.h"
#include "User.pb.h"

namespace SimpleChat {

Chatee::Chatee(std::unique_ptr<User> user) :
	user_(std::move(user)) {

}

Chatee::~Chatee() {
}

int Chatee::getUserId() const {
	return user_->id();
}

std::string Chatee::getUserName() const {
	return user_->name();
}

void Chatee::sendMessage(const std::string& message, const std::string& target) {
	auto chatMessage = std::make_unique<ChatMessage>();
	auto duration = std::chrono::system_clock::now().time_since_epoch();
	auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

	chatMessage->set_text(message);
	chatMessage->set_allocated_from(this->getSelf().release());
	chatMessage->set_timestamp(millis);

	if (!target.empty())
		chatMessage->set_allocated_target(this->getTarget(target).release());

	this->sendMessage(
		std::move(chatMessage)
	);
}

void Chatee::sendCommand(const std::string& command) {
	auto commandParser = std::make_unique<CommandParser>(command);
	auto chatCommand = commandParser->chatCommand(
		std::move(getSelf())
	);

	this->sendMessage(
		std::move(chatCommand)
	);
}

void Chatee::authorize(const std::string& password) {
	auto authorizeMessage = std::make_unique<ChatAuthorize>();
	authorizeMessage->set_allocated_from(getSelf().release());
	authorizeMessage->set_password(password);

	this->sendMessage(
		std::move(authorizeMessage)
	);
}

std::unique_ptr<ChatTarget> Chatee::getSelf() {
	if (!chatroom_.expired()) {
		return std::move(chatroom_.lock()->getTarget(user_->name()));
	}

	return nullptr;
}

std::unique_ptr<ChatTarget> Chatee::getTarget(const std::string& target) {
	if(!chatroom_.expired()) {
		return std::move(chatroom_.lock()->getTarget(target));
	}

	return nullptr;
}

}