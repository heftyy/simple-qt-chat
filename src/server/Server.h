#pragma once

#include <memory>

#include <QObject>
#include <QtNetwork/qhostaddress.h>

namespace SimpleChat {

class MessageDeserializer;

class UserJoinRequest;
class UserJoinResponse;

class ChatConnection;

class Server : public QObject {
public:
    Server() { };

    virtual void listen(quint16 port, QHostAddress ipAddress = QHostAddress::LocalHost) = 0;
    virtual ~Server() { }

protected:
    virtual QHostAddress getAddress() = 0;

private:
    virtual void handleUntypedMessage(const MessageDeserializer& deserializer,
                                      const std::shared_ptr<ChatConnection>& connection) = 0;

    virtual void handleMessage(std::unique_ptr<UserJoinRequest> joinRequest,
                               const std::shared_ptr<ChatConnection>& connection) = 0;

    virtual void handleMessage(std::unique_ptr<UserJoinResponse> joinResponse) = 0;
};

}