#include "MotdChatCommand.h"

namespace SimpleChat {

void MotdChatCommand::insertData(const std::vector<std::string>& arguments) {
    //! join all arguments because motd can contain spaces
    add_arguments(StringUtil::join(arguments, " "));
}

} // namespace SimpleChat
