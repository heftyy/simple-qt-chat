#include <gtest/gtest.h>

#include <chat/commands/CommandParser.h>
#include <ChatMessage.pb.h>

using namespace SimpleChat;

std::unique_ptr<ChatCommand> getCommand(const std::string& command) {
    auto target = std::make_unique<ChatTarget>();
    target->set_user_name("name1");
    target->set_user_id(1);

    auto parser = CommandParser(command);
    return parser.chatCommand(std::move(target));
}

TEST(command_test, AuthCommand) {
    auto cmd = getCommand("/auth frrr");

    EXPECT_EQ(cmd->type(), CommandType::AUTH);
}

TEST(command_test, MotdCommand) {
    auto cmd = getCommand("/motd frrr a ffff");

    EXPECT_EQ(cmd->type(), CommandType::MOTD);
    EXPECT_EQ(cmd->arguments(0), "frrr a ffff");
}

TEST(command_test, KickCommand) {
    auto cmd = getCommand("/kick frrr");

    EXPECT_EQ(cmd->type(), CommandType::KICK);
    EXPECT_EQ(cmd->arguments(0), "frrr");
}

TEST(command_test, MuteCommand) {
    auto cmd = getCommand("/mute frrr");

    EXPECT_EQ(cmd->type(), CommandType::MUTE);
    EXPECT_EQ(cmd->arguments(0), "frrr");
}