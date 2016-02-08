#pragma once

#include <memory>

namespace SimpleChat {

class AbstractMessage;
class ChatTarget;
	
class ChatConnection {
public:
	virtual bool sendMessage(std::unique_ptr<AbstractMessage> message) = 0;
    virtual bool isAlive() = 0;
};

}