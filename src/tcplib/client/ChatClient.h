#pragma once

#include <ChatMessage.pb.h>
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

class User;
class TcpChatConnection;
class Chatroom;
class Chatee;

class ChatClient : public Client {
    Q_OBJECT
public:
    ChatClient();

    virtual void sendCommand(const std::string& command);
    virtual void sendMessage(const std::string& text, const std::string& target);

    virtual bool login(const QString& address, quint16 port, const QString& name);
    virtual void logout();

signals:
    void chatMessageSignal(const QString& text, const QString& from, const QString& target);
    void chatInfoSignal(const QString& text);
    void chatMotdSignal(const QString& motd);

    void refreshChateeList();

private slots:
    void handleUntypedMessage(const QString& serializedData);
    void connectionLost();

    void displayError(QAbstractSocket::SocketError socketError) const;

private:
    QHostAddress serverAddress_;
    quint16 serverPort_;
    std::string clientName_;
    std::shared_ptr<TcpChatConnection> serverConnection_;
    std::shared_ptr<Chatroom> chatroom_;

    virtual bool connectToServer();

    void handleMessage(std::unique_ptr<UserJoinResponse> joinResponse);
    void handleMessage(std::unique_ptr<UserListResponse> listResponse);
    void handleMessage(std::unique_ptr<UserChange> userChange);
    void handleMessage(std::unique_ptr<ChatMessage> chatMessage);
    void handleMessage(std::unique_ptr<ChatroomChange> chatroomChange);
    void handleMessage(std::unique_ptr<GenericChatResponse> response);

    void join();
    void requestUserList();

    QString format(const std::string& info);
    QString format(const User& user, const char* info);
    QString format(const User& user, const std::string& info);
    QString format(const User& user, const QString& info);
};

} // SimpleChat namespace
