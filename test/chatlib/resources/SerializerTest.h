#pragma once

#include <gtest/gtest.h>
#include <communication/MessageSerializer.h>

#include "AbstractMessageTest.h"

class SerializerTest : public ::testing::Test {
    using ser = SimpleChat::MessageSerializer;

public:
    SerializerTest() : 
        serializerFail(validChatMessage()),
        serializerSuccess(invalidChatMessage()) {
    }    

protected:
    virtual void SetUp() override {
    }

    virtual void TearDown() override {
    }

    ser validChatMessage() const {
        return MessageSerializer(std::move(AbstractMessageTest::invalidChatMessage()));
    }

    ser invalidChatMessage() const {
        return MessageSerializer(std::move(AbstractMessageTest::validChatMessage()));
    }

    ser serializerSuccess;
    ser serializerFail;
};