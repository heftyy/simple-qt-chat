#include <gtest/gtest.h>

#include "resources/CommandParserTest.h"
#include <chat/commands/CommandParser.h>

using namespace SimpleChat;

TEST_F(CommandParserTest, MotdWorks) {
    CommandParser parser("/motd this is a new motd");

    auto cmd = parser.chatCommand();
    ASSERT_NE(cmd, nullptr);

    EXPECT_TRUE(cmd->IsInitialized());

    EXPECT_EQ(cmd->type(), CommandType::MOTD);
    EXPECT_EQ(cmd->arguments(0), "this is a new motd");
}

TEST_F(CommandParserTest, KickWorks) {
    CommandParser parser("/kick this_user");

    auto cmd = parser.chatCommand();
    ASSERT_NE(cmd, nullptr);

    EXPECT_TRUE(cmd->IsInitialized());

    EXPECT_EQ(cmd->type(), CommandType::KICK);
    EXPECT_EQ(cmd->arguments(0), "this_user");
}

TEST_F(CommandParserTest, MuteWorks) {
    CommandParser parser("/mute this_user");

    auto cmd = parser.chatCommand();
    ASSERT_NE(cmd, nullptr);

    EXPECT_TRUE(cmd->IsInitialized());

    EXPECT_EQ(cmd->type(), CommandType::MUTE);
    EXPECT_EQ(cmd->arguments(0), "this_user");
}

TEST_F(CommandParserTest, UnmuteWorks) {
    CommandParser parser("/unmute this_user");

    auto cmd = parser.chatCommand();
    ASSERT_NE(cmd, nullptr);

    EXPECT_TRUE(cmd->IsInitialized());

    EXPECT_EQ(cmd->type(), CommandType::UNMUTE);
    EXPECT_EQ(cmd->arguments(0), "this_user");
}

TEST_F(CommandParserTest, UnknownType) {
    CommandParser parser("/foo this_user");

    auto cmd = parser.chatCommand();
    EXPECT_EQ(cmd, nullptr);
}