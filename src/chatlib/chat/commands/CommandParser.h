#pragma once

#include <memory>

namespace SimpleChat {

class ChatTarget;
class ChatCommand;

/*!
 * Class that deals with every chat message starting with a '/' (except /w for whisper).
 * CommandParser stores the command and lets you get an instance of parsed command later.
 */
class CommandParser {
public:
    explicit CommandParser(const std::string& command);

    /*!
     * Creates and returns a correct instance of ChatCommand.
     * If the parsing fails this method returns nullptr.
     *
     * Supported command types:
     *   - mute
     *   - unmute
     *   - kick
     *   - motd
     *   - auth
     */
    std::unique_ptr<ChatCommand> chatCommand();

private:
    std::string command_;

    int getCommandType(const std::string& type) const;
};

} // SimpleChat namespace