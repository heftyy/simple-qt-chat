#pragma once

#include <memory>

#include <QObject>
#include <QtNetwork/qhostaddress.h>

namespace SimpleChat {

class MessageDeserializer;
class ChatConnection;

class Client : public QObject {
public:
    virtual void showChatWindow() = 0;

    virtual void requestUserList() = 0;
    virtual void sendCommand(std::string command) = 0;
    virtual void sendMessage(std::string text, std::string target) = 0;

    virtual void serverDialog() = 0;
    virtual void nameDialog() = 0;
    virtual void connect() = 0;
    virtual void join() = 0;

    virtual void handleUntypedMessage(const MessageDeserializer& deserializer,
                                      const std::shared_ptr<ChatConnection>& connection) = 0;

    virtual QHostAddress getServerAddress() = 0;
    virtual ~Client() {}
};

} // SimpleChat namespace