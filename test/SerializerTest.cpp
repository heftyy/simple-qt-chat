#include <gtest/gtest.h>

#include "resources/MessageTest.h"
#include "resources/AbstractMessageTest.h"
#include "resources/SerializerTest.h"

TEST_F(SerializerTest, SerializerWorks) {
    bool success;
    std::string result;

    std::tie(success, result) = getSerializer(AbstractMessageTest::build(MessageTest::validChatCommand())).serialize();
    EXPECT_TRUE(success);
}

TEST_F(SerializerTest, SerializerFails) {
    bool success;
    std::string result;

//    std::tie(success, result) = serializerFail.serialize();
//    EXPECT_FALSE(success);
}