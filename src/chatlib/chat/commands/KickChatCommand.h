#pragma once

#include "SpecificChatCommand.h"

namespace SimpleChat {

class KickChatCommand : public SpecificChatCommand {
public:
    virtual void insertData(const std::vector<std::string>& arguments) override;
};

} // namespace SimpleChat
