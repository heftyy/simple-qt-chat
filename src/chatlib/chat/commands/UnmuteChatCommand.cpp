#include "UnmuteChatCommand.h"

namespace SimpleChat {

void UnmuteChatCommand::insertData(const std::vector<std::string>& arguments) {
    if(arguments.size() != 1) {
        return;
    }
    // argument is a name
    add_arguments(arguments[0]);
}

}
