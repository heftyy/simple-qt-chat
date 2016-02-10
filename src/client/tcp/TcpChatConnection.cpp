#include <QtNetwork>
#include <iostream>

#include <communication/AbstractMessage.h>
#include <communication/MessageSerializer.h>

#include "TcpChatConnection.h"

namespace SimpleChat {

TcpChatConnection::TcpChatConnection(const std::shared_ptr<QTcpSocket>& socket_) :
        blockSize(0),
        socket_(socket_) {

}

bool TcpChatConnection::sendMessage(std::unique_ptr<AbstractMessage> message) {
    if (!isAlive())
        return false;

    while(socket_->bytesToWrite() > 0) {
        socket_->waitForBytesWritten(1000);
    }

    MessageSerializer serializer(
        std::move(message),
        getIdent(),
        ""
        );

    bool success;
    std::string result;
    std::tie(success, result) = serializer.serialize();

    if(!success)
        return false;

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_5);

    out << static_cast<quint16>(0);
    out << QString::fromStdString(result);
    out.device()->seek(0);
    out << static_cast<quint16>(block.size() - sizeof(quint16));

    socket_->write(block);

    return true;
}

bool TcpChatConnection::isAlive() const {
    return socket_->isOpen() && socket_->isValid() &&
        socket_->isWritable() && socket_->isReadable();
}

std::string TcpChatConnection::getIdent() const {
    auto str = socket_->peerAddress().toString() + ":" + QString::number(socket_->peerPort());
    return str.toStdString();
}

std::shared_ptr<QTcpSocket> TcpChatConnection::socket() {
    return socket_;
}

void TcpChatConnection::setChatee(const std::shared_ptr<Chatee>& chatee) {
    chatee_ = chatee;
}

std::shared_ptr<Chatee> TcpChatConnection::chatee() const {
    return chatee_.lock();
}

} //SimpleChat namespace
