#pragma once

#include <gtest/gtest.h>
#include <communication/MessageSerializer.h>
#include <communication/MessageDeserializer.h>

#include "SerializerTest.h"

class DeserializerTest : public ::testing::Test {
    using serializer = SimpleChat::MessageSerializer;
    using deserializer = SimpleChat::MessageDeserializer;

protected:
    virtual void SetUp() override {
    }

    virtual void TearDown() override {
    }

    template<typename T>
    deserializer getDeserializer(T&& msg) {
        bool success;
        std::string result;

        serializer s(std::forward<T>(msg));
        std::tie(success, result) = s.serialize();

        return deserializer(result);
    }

    template<typename T>
    void deserializerWorks(std::unique_ptr<AbstractMessage> msg, int expectedType) {
        deserializer d1 = getDeserializer(std::move(msg));
        EXPECT_EQ(d1.type(), expectedType);
        auto m1 = d1.getMessage<T>();
        EXPECT_TRUE(m1->IsInitialized());
    }

    template<typename T>
    void deserializerFails(std::unique_ptr<AbstractMessage> msg) {
        deserializer d1 = getDeserializer(std::move(msg));
        EXPECT_EQ(d1.type(), -1);
        auto m1 = d1.getMessage<T>();
        EXPECT_EQ(m1, nullptr);
    }
};