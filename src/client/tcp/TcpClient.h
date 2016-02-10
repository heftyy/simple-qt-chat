#pragma once

#include "Client.h"

namespace SimpleChat {

class MessageDeserializer;

class UserJoinRequest;
class UserJoinResponse;

class UserListRequest;
class UserListResponse;

class UserChange;

class ChatMessage;
class ChatAuthorize;
class ChatCommand;

class TcpChatConnection;
class Chatee;

class TcpClient : public Client {
public:
    virtual void showChatWindow() override;

    virtual void requestUserList() override;
    virtual void sendCommand(std::string command) override;
    virtual void sendMessage(std::string text, std::string target) override;

    virtual void serverDialog() override;
    virtual void nameDialog() override;
    virtual void connect() override;
    virtual void join() override;

    virtual void handleUntypedMessage(const MessageDeserializer& deserializer,
                                      const std::shared_ptr<ChatConnection>& connection) override;

    virtual void handleMessage(std::unique_ptr<UserJoinResponse> joinRequest);

    virtual void handleMessage(std::unique_ptr<UserListResponse> listRequest);

    virtual QHostAddress getServerAddress() override;

    QHostAddress serverAddress_;
    quint16 serverPort_;
    std::string clientName_;
    std::shared_ptr<TcpChatConnection> serverConnection_;

    void displayError(QAbstractSocket::SocketError socketError);

private slots:
    void dataReceived();
};

} // SimpleChat namespace
