#pragma once

#include <memory>
#include <string>

namespace SimpleChat {

class AbstractMessage;
class ChatTarget;

class ChatConnection {
public:
    virtual ~ChatConnection() {}

    virtual bool sendMessage(std::unique_ptr<AbstractMessage> message) = 0;
    virtual bool isAlive() const = 0;
    virtual std::string getIdent() const = 0;
};

} // SimpleChat namespace