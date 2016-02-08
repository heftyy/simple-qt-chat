#include <gtest/gtest.h>

#include "resources/SerializerTest.h"

TEST_F(SerializerTest, IsInitialized) {
    EXPECT_NE(serializer1, nullptr);
    EXPECT_NE(serializer2, nullptr);
}

TEST_F(SerializerTest, SerializationWorks) {
    bool success;
    std::string result;

    std::tie(success, result) = serializer1->serialize();
    EXPECT_TRUE(success);
}

TEST_F(SerializerTest, SerializationFails) {
    bool success;
    std::string result;

    std::tie(success, result) = serializer2->serialize();
    EXPECT_FALSE(success);
}