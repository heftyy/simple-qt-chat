#pragma once

#include <string>
#include <vector>

#include "ChatMessage.pb.h"

namespace SimpleChat {

class SpecificChatCommand : public ChatCommand {
public:
    virtual void insertData(const std::vector<std::string>& arguments) = 0;
    virtual ~SpecificChatCommand() { }
};

} // namespace SimpleChat