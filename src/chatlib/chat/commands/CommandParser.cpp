#include <stdexcept>

#include "../../util/StringUtil.h"
#include "CommandParser.h"
#include "ChatMessage.pb.h"
#include "SpecificChatCommand.h"
#include "MotdChatCommand.h"
#include "MuteChatCommand.h"
#include "KickChatCommand.h"

namespace SimpleChat {

CommandParser::CommandParser(const std::string& command):
    command_(command) {

}

std::unique_ptr<ChatCommand> CommandParser::chatCommand(std::unique_ptr<ChatTarget> from) {
    if(command_.empty())
        return nullptr;

    if(command_.at(0) == '/')
        command_ = command_.substr(1);

    auto commandArgs = StringUtil::split(command_, ' ');
    if (commandArgs.size() < 2)
        return nullptr;

    auto commandType = getCommandType(commandArgs[0]);
    if(commandType == CommandType::UNKNOWN)
        return nullptr;

    // remove the command type from vector
    commandArgs.erase(commandArgs.begin());

    std::unique_ptr<SpecificChatCommand> chatCommand;

    if(CommandType::MOTD == commandType)
        chatCommand = std::make_unique<MotdChatCommand>();
    else if(CommandType::KICK == commandType)
        chatCommand = std::make_unique<KickChatCommand>();
    else if(CommandType::MUTE == commandType)
        chatCommand = std::make_unique<MuteChatCommand>();

    chatCommand->set_type(static_cast<CommandType>(commandType));
    chatCommand->insertData(commandArgs);

    chatCommand->set_allocated_from(from.release());

    return std::move(chatCommand);
}

int CommandParser::getCommandType(const std::string& type) const {
    if (type == "mute") {
        return CommandType::MUTE;
    }
    if (type == "kick") {
        return CommandType::KICK;
    }
    if (type == "motd") {
        return CommandType::MOTD;
    }

    return CommandType::UNKNOWN;
}

} // SimpleChat namespace
