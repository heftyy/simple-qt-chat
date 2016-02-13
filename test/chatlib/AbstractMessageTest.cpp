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

TEST_F(AbstractMessageTest, CloneMessage) {
    auto msg1 = validAbstractMessage();
    auto msg2 = msg1->clone();

    EXPECT_EQ(msg1->type(), msg2->type());
    EXPECT_EQ(msg1->serialize(), msg2->serialize());
}