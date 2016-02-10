#pragma once

#include <gtest/gtest.h>
#include <communication/MessageDeserializer.h>

#include "SerializerTest.h"

class DeserializerTest : public ::testing::Test {
    using deser = SimpleChat::MessageDeserializer;

protected:
    virtual void SetUp() override {
        // success
        deserializerSuccess = getValidMessage();

        // fail
        deserializerFail = getInvalidMessage();
    }

    virtual void TearDown() override {
        deserializerSuccess.reset();
        deserializerFail.reset();
    }

    std::shared_ptr<deser> deserializerSuccess;
    std::shared_ptr<deser> deserializerFail;

public:
    static std::shared_ptr<deser> getValidMessage() {
        bool success;
        std::string result;

        std::tie(success, result) = SerializerTest::getValidMessage()->serialize();

        return std::make_shared<deser>(
                result
        );
    }

    static std::shared_ptr<deser> getInvalidMessage() {
        bool success;
        std::string result;

        std::tie(success, result) = SerializerTest::getInvalidMessage()->serialize();

        return std::make_shared<deser>(
                result
        );
    }
};