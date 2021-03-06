#pragma once

#include <memory>

#include <QtNetwork>

#include <server/ChatServer.h>

namespace SimpleChat {

class Chatroom;
class TcpChatConnection;

/*!
 * Implements a ChatServer using QTcpServer.
 */
class TcpChatServer : public QObject, public ChatServer {
    Q_OBJECT
public:
    explicit TcpChatServer(const std::string& password, QObject* parent = nullptr);
    ~TcpChatServer();

    /*!
     * Starts this server and connects newConnection SIGNAL to connectionEstablished.
     */
    virtual void listen(quint16 port, const QHostAddress& ipAddress);

private slots:
    void handleUntypedMessage(const QString& serializedMessage);

    void connectionEstablished();
    void connectionLost();

private:
    QTcpServer* tcpServer_;

    void start(quint16 port, const QHostAddress& ipAddress);
};

} // SimpleChat namespace
