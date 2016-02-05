#pragma once

#include <memory>

#include "proto/ChatMessage.pb.h"
#include "AbstractMessage.h"

namespace SimpleChat {

class Message {
    using protoMsg = google::protobuf::Message;

private:
    std::unique_ptr<BasicMessage> message_;
};

}

