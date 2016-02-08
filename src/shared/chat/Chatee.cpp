#include <chrono>
#include <cctype>

#include "../communication/AbstractMessage.h"
#include "Chatee.h"
#include "Chatroom.h"
#include "CommandParser.h"
#include "ChatConnection.h"

#include "ChatMessage.pb.h"
#include "User.pb.h"

namespace SimpleChat {

Chatee::Chatee(std::unique_ptr<User> user,
               const std::shared_ptr<ChatConnection>& connection)
        : user_(std::move(user)),
          connection_(connection) {

}

Chatee::~Chatee() {
}

bool Chatee::sendMessage(std::unique_ptr<AbstractMessage> message) {
    if(connection_) {
        return connection_->sendMessage(std::move(message));
    }

    return false;
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

User& Chatee::user() {
    return *user_;
}

}