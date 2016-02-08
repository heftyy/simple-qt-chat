#include <stdexcept>

#include "CommandParser.h"
#include "../util/StringUtil.h"
#include <proto/ChatMessage.pb.h>

namespace SimpleChat {

CommandParser::CommandParser(const std::string& command): 
    command_(command) {

}

std::unique_ptr<ChatCommand> CommandParser::chatCommand(std::unique_ptr<ChatTarget> chatTarget) const {
    auto chatCommand = std::make_unique<ChatCommand>();

    auto commandArgs = StringUtil::split(command_, ' ');
    if (commandArgs.size() < 2)
        return nullptr;

    auto commandType = getCommandType(commandArgs[0]);
    chatCommand->set_type(commandType);

    for (auto i = 0; i < commandArgs.size(); i++)
        chatCommand->set_arguments(i - 1, commandArgs[i]);

    chatCommand->set_allocated_from(chatTarget.release());

    return std::move(chatCommand);
}

CommandType CommandParser::getCommandType(const std::string& type) const {
    if (type == "mute") {
        return CommandType::MUTE;
    }
    if (type == "kick") {
        return CommandType::KICK;
    }
    if (type == "motd") {
        return CommandType::MOTD;
    }

    throw std::runtime_error("Unrecognized command type");
}

} // SimpleChat namespace
