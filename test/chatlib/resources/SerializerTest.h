#pragma once

#include <gtest/gtest.h>
#include <communication/MessageSerializer.h>

#include "AbstractMessageTest.h"

class SerializerTest : public ::testing::Test {
    using ser = SimpleChat::MessageSerializer;

protected:
    virtual void SetUp() override {
        // success
        serializerSuccess = getValidMessage();

        // fail
        serializerFail = getInvalidMessage();
    }

    virtual void TearDown() override {
        serializerSuccess.reset();
        serializerFail.reset();
    }

    std::shared_ptr<ser> serializerSuccess;
    std::shared_ptr<ser> serializerFail;

public:
    static std::shared_ptr<ser> getValidMessage() {
        return std::make_shared<ser>(
                AbstractMessageTest::validAbstractMessage());
    }

    static std::shared_ptr<ser> getInvalidMessage() {
        return std::make_shared<ser>(
                AbstractMessageTest::invalidAbstractMessage());
    }
};