#pragma once

#include <memory>

#include <QtNetwork>

#include <server/ChatServer.h>

namespace SimpleChat {

class Chatroom;
class TcpChatConnection;

using ConnectionsMap = std::map<std::string, std::shared_ptr<TcpChatConnection>>;

class TcpChatServer : public QObject, public ChatServer {
    Q_OBJECT
public:
    explicit TcpChatServer(const std::string& password);

    virtual void listen(quint16 port, const QHostAddress& ipAddress);

private slots:
    void handleUntypedMessage(const QString& serializedMessage);

    void connectionEstablished();
    void connectionLost();

private:
    ConnectionsMap connections_;
    std::unique_ptr<QTcpServer> tcpServer_;

    void openSession(quint16 port, const QHostAddress& ipAddress);
};

} //SimpleChat namespace
