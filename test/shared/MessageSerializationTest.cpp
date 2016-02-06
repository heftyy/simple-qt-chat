#include "gtest/gtest.h"
#include "communication/AbstractMessage.h"
#include "User.pb.h"

TEST(message_serialization_test, serialize_message) {
	SimpleChat::User u;
	u.set_id(1);
	u.set_name("First user");
	u.set_status(SimpleChat::ACTIVE);
	EXPECT_TRUE(u.IsInitialized());
}