#include <gtest/gtest.h>

#include <communication/AbstractMessage.h>
#include <User.pb.h>
#include <NetworkMessage.pb.h>

SimpleChat::NetworkMessage createNetworkMessage() {
    SimpleChat::UserJoinRequest userJoinRequest;
    userJoinRequest.set_name("first_user");

    EXPECT_TRUE(userJoinRequest.IsInitialized());

    SimpleChat::NetworkMessage networkMessage;

    networkMessage.set_serialized_data(userJoinRequest.SerializeAsString());

    auto networkMessageHeader = networkMessage.mutable_header();
    networkMessageHeader->set_type(SimpleChat::USER_JOIN_REQUEST);

    EXPECT_TRUE(networkMessageHeader->IsInitialized());
    EXPECT_TRUE(networkMessage.IsInitialized());

    return networkMessage;
}

TEST(network_message_test, SerializeWorks) {
    createNetworkMessage();
}

TEST(network_message_test, DeserializeWorks) {
    auto serializedMessage = createNetworkMessage().SerializeAsString();

    SimpleChat::NetworkMessage networkMessage;
    EXPECT_TRUE(networkMessage.ParseFromString(serializedMessage));

    EXPECT_TRUE(networkMessage.IsInitialized());

    EXPECT_EQ(networkMessage.header().type(), SimpleChat::NetworkMessageType::USER_JOIN_REQUEST);

    SimpleChat::UserJoinRequest userJoinRequest;
    userJoinRequest.ParseFromString(networkMessage.serialized_data());
    EXPECT_TRUE(userJoinRequest.IsInitialized());
}