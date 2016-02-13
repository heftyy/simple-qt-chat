#pragma once

#include <memory>

namespace SimpleChat {

class ChatTarget;
class ChatCommand;

class CommandParser {
public:
    explicit CommandParser(const std::string& command);

    std::unique_ptr<ChatCommand> chatCommand(std::unique_ptr<ChatTarget> from);

private:
    std::string command_;

    int getCommandType(const std::string& type) const;
};

} // SimpleChat namespace