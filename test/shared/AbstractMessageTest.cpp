#include <gtest/gtest.h>

#include "resources/AbstractMessage.h"

TEST_F(AbstractMessage, InvalidMessage) {
    auto msg = invalidAbstractMessage();

    EXPECT_FALSE(msg->isInitialized());
}

TEST_F(AbstractMessage, ValidMessage) {
    auto msg = validAbstractMessage();

    EXPECT_TRUE(msg->isInitialized());
}