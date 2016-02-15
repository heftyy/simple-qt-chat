#include "AuthChatCommand.h"

namespace SimpleChat {

void AuthChatCommand::insertData(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1) {
        return;
    }
    // argument is a password
    add_arguments(arguments[0]);
}

} // SimpleChat namespace 