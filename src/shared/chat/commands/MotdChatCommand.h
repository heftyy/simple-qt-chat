#pragma once

#include <string>

#include "../../util/StringUtil.h"
#include "SpecificChatCommand.h"

namespace SimpleChat {

class MotdChatCommand : public SpecificChatCommand {
public:
    virtual void insertData(const std::vector<std::string>& arguments) override;
};

} // namespace SimpleChat
