#include <gtest/gtest.h>

#include "resources/AbstractMessageTest.h"

TEST_F(AbstractMessageTest, InvalidMessage) {
    auto msg = invalidAbstractMessage();

    EXPECT_FALSE(msg->isInitialized());
}

TEST_F(AbstractMessageTest, ValidMessage) {
    auto msg = validAbstractMessage();

    EXPECT_TRUE(msg->isInitialized());
}