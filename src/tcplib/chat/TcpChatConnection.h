#pragma once

#include <QObject>
#include <chat/ChatConnection.h>

QT_BEGIN_NAMESPACE
class QTcpSocket;
QT_END_NAMESPACE

namespace SimpleChat {

class MessageDeserializer;
class Chatee;

class TcpChatConnection : public QObject,
                          public ChatConnection,
                          public std::enable_shared_from_this<TcpChatConnection> {
    Q_OBJECT
public:
    explicit TcpChatConnection(const std::shared_ptr<QTcpSocket>& socket_);

    void init();

    virtual bool sendMessage(std::unique_ptr<AbstractMessage> message) override;
    virtual bool isAlive() const override;
    virtual std::string getIdent() const override;

    virtual void setChatee(const std::shared_ptr<Chatee>& chatee) override;
    virtual std::shared_ptr<Chatee> chatee() const override;

    std::shared_ptr<QTcpSocket> socket();

signals:
    void dataReceivedSignal(const QString& serializedMessage);
    void disconnectSignal();

private slots:
    void disconnected();
    void readyRead();

private:
    quint16 blockSize_;

    std::shared_ptr<QTcpSocket> socket_;
    std::weak_ptr<Chatee> chatee_;
};

} //SimpleChat namespace



