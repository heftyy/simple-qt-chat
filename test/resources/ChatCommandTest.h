#pragma once

#include <gtest/gtest.h>
#include <communication/Message.h>
#include <chat/commands/CommandParser.h>
#include <ChatMessage.pb.h>

using namespace SimpleChat;

class ChatCommandTest : public ::testing::Test {
    using am = SimpleChat::AbstractMessage;
    using amPtr = std::unique_ptr<am>;

protected:
    virtual void SetUp() override {}

    virtual void TearDown() override {}

public:
    static std::unique_ptr<ChatCommand> getCommand(const std::string& command) {
        auto target = std::make_unique<ChatTarget>();
        target->set_user_name("name1");
        target->set_user_id(1);

        auto parser = CommandParser(command);
        return parser.chatCommand();
    }

};