#pragma once

#include <memory>
#include <string>

namespace SimpleChat {

class NetworkMessage;
class Message;

class MessageDeserializer {
public:
	explicit MessageDeserializer(const std::string& serializedMessage);

	std::unique_ptr<Message> getMessage();

private:
	std::string serializedMessage_;

	std::unique_ptr<NetworkMessage> getNetworkMessage() const;
};

}
