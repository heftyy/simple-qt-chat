#pragma once

#include <gtest/gtest.h>
#include <communication/MessageDeserializer.h>

#include "SerializerTest.h"

class DeserializerTest : public ::testing::Test {
public:
    DeserializerTest()
        : deserializerSuccess(validChatMessage()),
          deserializerFail(invalidChatMessage()) {}

    using deser = SimpleChat::MessageDeserializer;

protected:
    virtual void SetUp() override {
    }

    virtual void TearDown() override {
    }

    deser deserializerSuccess;
    deser deserializerFail;

    deser validChatMessage() const {
        bool success;
        std::string result;
        
        MessageSerializer s(std::move(AbstractMessageTest::validChatMessage()));
        std::tie(success, result) = s.serialize();

        return deser(result);
    }

    deser invalidChatMessage() const {
        bool success;
        std::string result;

        MessageSerializer s(std::move(AbstractMessageTest::invalidChatMessage()));
        std::tie(success, result) = s.serialize();

        return deser(result);
    }
};