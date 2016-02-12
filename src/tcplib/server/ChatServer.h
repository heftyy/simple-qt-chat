#pragma once

#include <memory>

#include "Server.h"

QT_BEGIN_NAMESPACE
class QTcpServer;
class QNetworkSession;
class QTcpSocket;
QT_END_NAMESPACE

namespace SimpleChat {

class Chatroom;
class TcpChatConnection;

using ConnectionsMap = std::map<std::string, std::shared_ptr<TcpChatConnection>>;

class ChatServer : public Server {
    Q_OBJECT
public:
    explicit ChatServer(const std::string& password);

    virtual void listen(quint16 port, QHostAddress ipAddress = QHostAddress::LocalHost) override;

    virtual QHostAddress getAddress() const override;

private slots:
    void connectionEstablished();
    void connectionLost();

    virtual void handleUntypedMessage(const QString& serializedMessage) override;

private:
    ConnectionsMap connections_;

    std::unique_ptr<QTcpServer> tcpServer_;
    std::shared_ptr<Chatroom> chatroom_;
    std::string password_;

    void openSession(quint16 port, QHostAddress ipAddress);

    virtual void handleMessage(std::unique_ptr<UserJoinRequest> joinRequest,
                               const std::shared_ptr<ChatConnection>& connection) override;

    virtual void handleMessage(std::unique_ptr<UserListRequest> listRequest,
                               const std::shared_ptr<Chatee>& sender) override;

    virtual void handleMessage(std::unique_ptr<UserChange> userChange,
                               const std::shared_ptr<Chatee>& sender) override;

    virtual void handleMessage(std::unique_ptr<ChatMessage> chatMessage,
                               const std::shared_ptr<Chatee>& sender) override;

    virtual void handleMessage(std::unique_ptr<ChatAuthorize> chatAuthorize,
                               const std::shared_ptr<Chatee>& sender) override;

    virtual void handleMessage(std::unique_ptr<ChatCommand> chatCommand,
                               const std::shared_ptr<Chatee>& sender) override;
};

} //SimpleChat namespace
