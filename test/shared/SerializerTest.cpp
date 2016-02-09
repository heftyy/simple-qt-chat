#include <gtest/gtest.h>

#include "resources/SerializerTest.h"

TEST_F(SerializerTest, IsInitialized) {
    EXPECT_NE(serializerSuccess, nullptr);
    EXPECT_NE(serializerFail, nullptr);
}

TEST_F(SerializerTest, SerializerWorks) {
    bool success;
    std::string result;

    std::tie(success, result) = serializerSuccess->serialize();
    EXPECT_TRUE(success);
}

TEST_F(SerializerTest, SerializerFails) {
    bool success;
    std::string result;

    std::tie(success, result) = serializerFail->serialize();
    EXPECT_FALSE(success);
}