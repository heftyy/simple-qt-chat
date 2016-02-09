#pragma once

#include <iostream>
#include <memory>

#include <QObject>

#include "Server.h"

QT_BEGIN_NAMESPACE
class QTcpServer;
class QNetworkSession;
class QTcpSocket;
QT_END_NAMESPACE

namespace SimpleChat {

class Chatroom;
class TcpChatConnection;

class TcpServer : public Server {
    Q_OBJECT
public:
    TcpServer();

    virtual void listen(quint16 port, QHostAddress ipAddress = QHostAddress::LocalHost) override;

    virtual void handleUntypedMessage(const MessageDeserializer& deserializer,
                                      const std::shared_ptr<ChatConnection>& connection) override;

protected:
    virtual QHostAddress getAddress() override;

    private slots:
    void connectionEstablished() const;
    void dataReceived(const std::shared_ptr<TcpChatConnection>& tcpSocket);

private:
    std::shared_ptr<QTcpServer> tcpServer_;
    std::shared_ptr<Chatroom> chatroom_;

    void openSession(quint16 port, QHostAddress ipAddress);

    virtual void handleMessage(std::unique_ptr<UserJoinRequest> joinRequest,
                               const std::shared_ptr<ChatConnection>& connection) override;

    virtual void handleMessage(std::unique_ptr<UserListRequest> listRequest,
                               const std::shared_ptr<ChatConnection>& connection) override;

    virtual void handleMessage(std::unique_ptr<UserJoinResponse> joinResponse) override;
};

} //SimpleChat namespace