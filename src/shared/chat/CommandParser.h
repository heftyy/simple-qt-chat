#pragma once

#include <string>
#include <memory>

#include "ChatMessage.pb.h"

namespace SimpleChat {
	
class CommandParser {
public:
	explicit CommandParser(const std::string& command);

	std::unique_ptr<ChatCommand> chatCommand(std::unique_ptr<ChatTarget> chatTarget) const;

private:
	std::string command_;

	CommandType getCommandType(const std::string& type) const;
};

}