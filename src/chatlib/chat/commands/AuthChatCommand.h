#pragma once

#include "SpecificChatCommand.h"

namespace SimpleChat {

class AuthChatCommand : public SpecificChatCommand {
public:
    void insertData(const std::vector<std::string>& arguments) override;
};
    
} // SimpleChat namespace