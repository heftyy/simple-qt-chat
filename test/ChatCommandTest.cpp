#include <gtest/gtest.h>

#include <chat/commands/CommandParser.h>
#include <ChatMessage.pb.h>
#include "resources/ChatCommandTest.h"

using namespace SimpleChat;

TEST_F(ChatCommandTest, AuthCommand) {
    auto cmd = getCommand("/auth frrr");

    EXPECT_EQ(cmd->type(), CommandType::AUTH);
}

TEST_F(ChatCommandTest, MotdCommand) {
    auto cmd = getCommand("/motd frrr a ffff");

    EXPECT_EQ(cmd->type(), CommandType::MOTD);
    EXPECT_EQ(cmd->arguments(0), "frrr a ffff");
}

TEST_F(ChatCommandTest, KickCommand) {
    auto cmd = getCommand("/kick frrr");

    EXPECT_EQ(cmd->type(), CommandType::KICK);
    EXPECT_EQ(cmd->arguments(0), "frrr");
}

TEST_F(ChatCommandTest, MuteCommand) {
    auto cmd = getCommand("/mute frrr");

    EXPECT_EQ(cmd->type(), CommandType::MUTE);
    EXPECT_EQ(cmd->arguments(0), "frrr");
}

TEST_F(ChatCommandTest, UnmuteCommand) {
    auto cmd = getCommand("/unmute frrr");

    EXPECT_EQ(cmd->type(), CommandType::UNMUTE);
    EXPECT_EQ(cmd->arguments(0), "frrr");
}