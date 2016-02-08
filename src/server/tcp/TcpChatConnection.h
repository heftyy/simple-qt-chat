#pragma once

#include <QObject>

#include <chat/ChatConnection.h>

QT_BEGIN_NAMESPACE
class QTcpSocket;
QT_END_NAMESPACE

namespace SimpleChat {

class TcpChatConnection : public ChatConnection {
public:
    quint16 blockSize;

    explicit TcpChatConnection(const std::shared_ptr<QTcpSocket>& socket_);

    virtual bool sendMessage(std::unique_ptr<AbstractMessage> message) override;
    virtual bool isAlive() override;

    std::shared_ptr<QTcpSocket> socket();

private:
    std::shared_ptr<QTcpSocket> socket_;
};

}



