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
    Q_OBJECT
public:
    virtual void showChatWindow();

    virtual void requestUserList();
    virtual void sendCommand(std::string command);
    virtual void sendMessage(std::string text, std::string target);

    virtual void serverDialog();
    virtual void nameDialog();
    virtual void connectToHost();
    virtual void disconnectFromHost();
    virtual void join();

    virtual void handleMessage(std::unique_ptr<UserJoinResponse> joinResponse);
    virtual void handleMessage(std::unique_ptr<UserListResponse> joinResponse);
    virtual void handleMessage(std::unique_ptr<UserChange> userChange);
    virtual void handleMessage(std::unique_ptr<ChatMessage> chatMessage);

    virtual QHostAddress getServerAddress();

    QHostAddress serverAddress_;
    quint16 serverPort_;
    std::string clientName_;
    std::shared_ptr<TcpChatConnection> serverConnection_;

    void displayError(QAbstractSocket::SocketError socketError) const;

private slots:
    virtual void handleUntypedMessage(QString serializedData,
                                      std::string ident);

    void connectionLost(std::string ident);
};

} // SimpleChat namespace
