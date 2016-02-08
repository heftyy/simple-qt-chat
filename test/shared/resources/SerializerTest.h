#include <gtest/gtest.h>
#include <communication/MessageSerializer.h>

#include "AbstractMessage.h"

class SerializerTest : public ::testing::Test {
	using ser = SimpleChat::MessageSerializer;

protected:
	virtual void SetUp() override {
		// success
		serializer1 = std::make_shared<ser>(
			AbstractMessage::validAbstractMessage(),
			"127.0.0.1:4444",
			"127.0.0.1:4445");

		// fail
		serializer2 = std::make_shared<ser>(
			AbstractMessage::invalidAbstractMessage(),
			"127.0.0.1:4444",
			"127.0.0.1:4445");
	}

	virtual void TearDown() override {
		serializer1.reset();
		serializer2.reset();
	}

	std::shared_ptr<ser> serializer1;
	std::shared_ptr<ser> serializer2;
};