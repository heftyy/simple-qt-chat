#pragma once

#include <memory>
#include <string>

namespace SimpleChat {

class AbstractMessage;
class Chatee;

/*!
 * Interface for connections.
 */
class ChatConnection {
public:
    virtual ~ChatConnection() {}

    virtual bool sendMessage(std::unique_ptr<AbstractMessage> message) = 0;
    virtual bool isAlive() const = 0;

    /*!
     * Ident of a connection is an ip address of the peer and port (eg 192.168.1.15:23415)
     */
    virtual std::string getIdent() const = 0;

    virtual void disconnectFromHost() = 0;

    virtual void setChatee(const std::shared_ptr<Chatee>& chatee) = 0;
    virtual std::shared_ptr<Chatee> chatee() const = 0;
};

} // SimpleChat namespace