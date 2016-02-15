#pragma once

#include <string>
#include <vector>

#include "ChatMessage.pb.h"

namespace SimpleChat {

/*!
 *  Interface for all chat commands.
 */
class SpecificChatCommand : public ChatCommand {
public:
    /*! Checks if the argument vector contains all necessary strings
     * and adds arguments to the ChatCommand protobuf class.
     */
    virtual void insertData(const std::vector<std::string>& arguments) = 0;
    virtual ~SpecificChatCommand() { }
};

} // namespace SimpleChat