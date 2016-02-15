#include "KickChatCommand.h"

namespace SimpleChat {

void KickChatCommand::insertData(const std::vector<std::string>& arguments) {
    if(arguments.size() != 1) {
        return;
    }
    //! argument is a name
    add_arguments(arguments[0]);
}

} // namespace SimpleChat
