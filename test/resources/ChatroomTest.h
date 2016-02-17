#pragma once

#include <gtest/gtest.h>
#include <chat/Chatroom.h>

class ChatroomTest : public ::testing::Test {
    using room = SimpleChat::Chatroom;

protected:
    virtual void SetUp() override {
        room1 = std::make_shared<room>();
    }

    virtual void TearDown() override {
        room1.reset();
    }

    /*!
     * chatroom has to be a shared_ptr because it uses std::shared_from_this()
     */
    std::shared_ptr<room> room1;
};