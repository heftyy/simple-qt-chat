#pragma once

#include <memory>
#include <string>

#include <google/protobuf/message.h>

namespace SimpleChat {

class User;
class Chatroom;
class ChatConnection;

class Chatee
{
public:
	explicit Chatee(std::unique_ptr<User> user);
	virtual ~Chatee();

	int getUserId() const;
	std::string getUserName() const;

	virtual void setAuthorized(bool authorized) = 0;
	virtual void sendMessage(std::unique_ptr<AbstractMessage> message) = 0;

	void sendMessage(const std::string& message, const std::string& target = "");
	void sendCommand(const std::string& command);
	void authorize(const std::string& password);	

private:
	std::unique_ptr<User> user_;
	std::weak_ptr<Chatroom> chatroom_;

	virtual std::unique_ptr<ChatTarget> getSelf();
	virtual std::unique_ptr<ChatTarget> getTarget(const std::string& target);

	virtual void sendMessage(std::unique_ptr<google::protobuf::Message> message) = 0;
};

}