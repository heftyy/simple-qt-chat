#pragma once

#include <QObject>
#include <chat/ChatConnection.h>

QT_BEGIN_NAMESPACE
class QTcpSocket;
QT_END_NAMESPACE

namespace SimpleChat {

class MessageDeserializer;
class Chatee;

/*!
 * A ChatConnection implementation using Qt.
 * Holds a QTcpSocket to write to and read from.
 *
 * TcpChatConnection is a QObject and emits signals.
 */
class TcpChatConnection : public QObject,
                          public ChatConnection {
    Q_OBJECT
public:
    explicit TcpChatConnection(QTcpSocket* socket, QObject* parent = nullptr);
    ~TcpChatConnection();

    void init() const;

    /*!
     * Non blocking.
     * Serializes the AbstractMessage interface and writes to socket.
     *
     * Returns true if serialization succeeded, doesn't wait for all
     * bytes to be sent.
     */
    virtual bool sendMessage(std::unique_ptr<AbstractMessage> message) override;
    virtual bool isAlive() const override;
    virtual std::string getIdent() const override;

    void disconnectFromHost() override;

    virtual void setChatee(const std::shared_ptr<Chatee>& chatee) override;
    virtual std::shared_ptr<Chatee> chatee() const override;

    QTcpSocket* socket() const;

signals:
    void dataReceivedSignal(const QString& serializedMessage);
    void disconnectSignal();

private slots:
    void disconnected();
    void readyRead();

private:
    quint16 blockSize_;

    QTcpSocket* socket_;
    std::weak_ptr<Chatee> chatee_;
};

} // SimpleChat namespace



