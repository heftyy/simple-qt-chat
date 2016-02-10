#pragma once

#include <QObject>

#include <chat/ChatConnection.h>

QT_BEGIN_NAMESPACE
class QTcpSocket;
QT_END_NAMESPACE

namespace SimpleChat {

class Chatee;

class TcpChatConnection : public ChatConnection {
public:
    quint16 blockSize;

    explicit TcpChatConnection(const std::shared_ptr<QTcpSocket>& socket_);

    virtual bool sendMessage(std::unique_ptr<AbstractMessage> message) override;
    virtual bool isAlive() const override;
    virtual std::string getIdent() const override;

    virtual void setChatee(const std::shared_ptr<Chatee>& chatee) override;
    virtual std::shared_ptr<Chatee> chatee() const override;

    std::shared_ptr<QTcpSocket> socket();

private:
    std::shared_ptr<QTcpSocket> socket_;
    std::weak_ptr<Chatee> chatee_;
};

} //SimpleChat namespace



