#pragma once

#include <map>
#include <memory>
#include <mutex>

namespace SimpleChat {

class Chatee;
class User;
class AbstractMessage;
class ChatTarget;
class ChatMessage;
class ChatCommand;
class ChatAuthorize;

class Chatroom
{
public:
	explicit Chatroom();
	~Chatroom();

	void chateeJoined(std::unique_ptr<User> user);
	void chateeLeft(std::unique_ptr<User> user);

	void messageReceived(std::unique_ptr<ChatMessage> chatMessage);
	void commandReceived(std::unique_ptr<ChatCommand> chatCommand);
	void authorizeReceived(std::unique_ptr<ChatAuthorize> chatAuthorize);


	std::unique_ptr<ChatTarget> getTarget(const std::string& userName);

private:
	std::mutex chatees_mutex_;
	std::map<std::string, std::unique_ptr<Chatee>> chatees_;
	std::string motd_;

	void insertChatee(std::unique_ptr<Chatee> chatee);
	void removeChatee(const std::string& userName);

	void propageMessage(std::unique_ptr<AbstractMessage> abstractMessage);
};

}