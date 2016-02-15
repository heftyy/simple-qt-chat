#pragma once

#include "SpecificChatCommand.h"

namespace SimpleChat {

class UnmuteChatCommand : public SpecificChatCommand {
public:
    virtual void insertData(const std::vector<std::string>& arguments) override;
};

}
