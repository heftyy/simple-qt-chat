#include <gtest/gtest.h>
#include <communication/Message.h>
#include "User.pb.h"

class AbstractMessage : public ::testing::Test {
    using am = SimpleChat::AbstractMessage;
    using m = SimpleChat::Message<SimpleChat::UserJoinRequest>;

protected:
    virtual void SetUp() override {}

    virtual void TearDown() override {}

public:
    static std::unique_ptr<am> invalidAbstractMessage() {
        auto userJoinRequest = std::make_unique<SimpleChat::UserJoinRequest>();

        return std::make_unique<m>(std::move(userJoinRequest), SimpleChat::USER_JOIN_REQUEST);
    }

    static std::unique_ptr<am> validAbstractMessage() {
        auto userJoinRequest = std::make_unique<SimpleChat::UserJoinRequest>();
        userJoinRequest->set_name("first_user");

        return std::make_unique<m>(std::move(userJoinRequest), SimpleChat::USER_JOIN_REQUEST);
    }
};