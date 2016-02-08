#pragma once

#include <map>
#include <memory>
#include <mutex>
#include <communication/AbstractMessage.h>

namespace SimpleChat {

class Chatee;
class ChatConnection;
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

    std::tuple<bool, std::string, std::shared_ptr<Chatee>>
            chateeJoined(const std::string& name,
                         const std::shared_ptr<ChatConnection>& connection);

    std::tuple<bool, std::string>
            chateeLeft(const std::string& name);

    bool sendMessage(const std::string& name,
                     std::unique_ptr<AbstractMessage> message);

	void incomingMessage(std::unique_ptr<ChatMessage> chatMessage);
	void incomingMessage(std::unique_ptr<ChatCommand> chatCommand);
	void incomingMessage(std::unique_ptr<ChatAuthorize> chatAuthorize);

    void propagateMessage(std::unique_ptr<AbstractMessage> abstractMessage);

	std::unique_ptr<ChatTarget> getTarget(const std::string& userName);

private:
    int nextUserId;

	std::mutex chatees_mutex_;
	std::map<std::string, std::shared_ptr<Chatee>> chatees_;
	std::string motd_;

    bool chateeExists(const std::string& name);

    void insertChatee(std::shared_ptr<Chatee> chatee);
	void removeChatee(const std::string& userName);
};

}