#include "MotdChatCommand.h"

namespace SimpleChat {

void MotdChatCommand::insertData(const std::vector<std::string>& arguments) {
    add_arguments(StringUtil::join(arguments, " "));
}

} // namespace SimpleChat
