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

	std::shared_ptr<room> room1;
};