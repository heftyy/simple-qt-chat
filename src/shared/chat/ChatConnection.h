#pragma once

#include <memory>

namespace SimpleChat {

class AbstractMessage;
class ChatTarget;
	
class ChatConnection {
public:
	virtual void sendMessage(std::unique_ptr<AbstractMessage> message) = 0;	
};

}