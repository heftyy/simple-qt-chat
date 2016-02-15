#include <stdexcept>

#include "../../util/StringUtil.h"
#include "CommandParser.h"
#include "ChatMessage.pb.h"
#include "SpecificChatCommand.h"
#include "MotdChatCommand.h"
#include "MuteChatCommand.h"
#include "UnmuteChatCommand.h"
#include "KickChatCommand.h"
#include "AuthChatCommand.h"

namespace SimpleChat {

CommandParser::CommandParser(const std::string& command):
    command_(command) {

}

std::unique_ptr<ChatCommand> CommandParser::chatCommand(std::unique_ptr<ChatTarget> from) {
    if(from == nullptr)
        return nullptr;

    if(command_.empty())
        return nullptr;

    if(command_.at(0) == '/')
        command_ = command_.substr(1);

    auto commandArgs = StringUtil::split(command_, ' ');
    if (commandArgs.size() < 2)
        return nullptr;

    auto commandType = getCommandType(commandArgs[0]);
    if(commandType == UNKNOWN)
        return nullptr;

    //! remove the command type from vector
    commandArgs.erase(commandArgs.begin());

    std::unique_ptr<SpecificChatCommand> chatCommand;

    if(MOTD == commandType)
        chatCommand = std::make_unique<MotdChatCommand>();
    else if(KICK == commandType)
        chatCommand = std::make_unique<KickChatCommand>();
    else if (MUTE == commandType)
        chatCommand = std::make_unique<MuteChatCommand>();
    else if (UNMUTE == commandType)
        chatCommand = std::make_unique<UnmuteChatCommand>();
    else if (AUTH == commandType)
        chatCommand = std::make_unique<AuthChatCommand>();
    else
        return nullptr;

    chatCommand->set_type(static_cast<CommandType>(commandType));
    chatCommand->insertData(commandArgs);

    chatCommand->set_allocated_from(from.release());

    return std::move(chatCommand);
}

int CommandParser::getCommandType(const std::string& type) const {
    if (type == "mute") {
        return MUTE;
    }
    if (type == "unmute") {
        return UNMUTE;
    }
    if (type == "kick") {
        return KICK;
    }
    if (type == "motd") {
        return MOTD;
    }
    if(type == "auth") {
        return AUTH;
    }

    return UNKNOWN;
}

} // SimpleChat namespace
