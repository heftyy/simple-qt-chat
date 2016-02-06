#pragma once

#include <map>
#include <memory>
#include <mutex>

namespace SimpleChat {

class Chatee;
class User;
class AbstractMessage;

class Chatroom
{
public:
	explicit Chatroom();
	~Chatroom();

	void chateeJoined(std::unique_ptr<User> user);
	void chateeLeft(std::unique_ptr<User> user);

	void newMessage(std::unique_ptr<AbstractMessage> abstractMessage);

private:
	std::mutex chatees_mutex_;
	std::map<int, std::unique_ptr<Chatee>> chatees_;

	void insertChatee(std::unique_ptr<Chatee> chatee);
	void removeChatee(int userId);
};

}