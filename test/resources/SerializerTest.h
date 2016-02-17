#pragma once

#include <gtest/gtest.h>
#include <communication/MessageSerializer.h>

class SerializerTest : public ::testing::Test {
    using serializer = SimpleChat::MessageSerializer;

protected:
    virtual void SetUp() override {
    }

    virtual void TearDown() override {
    }

    template<typename T>
    serializer getSerializer(T&& msg) const {
        return MessageSerializer(std::forward<T>(msg));
    }
};