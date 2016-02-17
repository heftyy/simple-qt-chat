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
};