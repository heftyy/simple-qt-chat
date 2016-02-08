#include <QtNetwork>

#include <communication/AbstractMessage.h>

#include "TcpChatConnection.h"

namespace SimpleChat {

TcpChatConnection::TcpChatConnection(const std::shared_ptr<QTcpSocket>& socket_)
        : socket_(socket_),
          blockSize(0) {

}

bool TcpChatConnection::sendMessage(std::unique_ptr<AbstractMessage> message) {
    if(!isAlive()) return false;

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_5);

    out << (quint16)0;
    out << QString::fromStdString(message->serialize());
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    socket_->write(block);

    return true;
}

bool TcpChatConnection::isAlive() {
    return false;
}

std::shared_ptr<QTcpSocket> TcpChatConnection::socket() {
    return socket_;
}

}
