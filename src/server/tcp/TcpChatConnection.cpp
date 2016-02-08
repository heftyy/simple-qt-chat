#include <QtNetwork>

#include <communication/AbstractMessage.h>
#include <communication/MessageSerializer.h>

#include "TcpChatConnection.h"

namespace SimpleChat {

TcpChatConnection::TcpChatConnection(const std::shared_ptr<QTcpSocket>& socket_)
        : socket_(socket_),
          blockSize(0) {

}

bool TcpChatConnection::sendMessage(std::unique_ptr<AbstractMessage> message) {
    if(!isAlive()) return false;

	MessageSerializer serializer(
		std::move(message),
		getIdent(),
		""
	);

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_5);

    out << static_cast<quint16>(0);
    out << QString::fromStdString(message->serialize());
    out.device()->seek(0);
    out << static_cast<quint16>(block.size() - sizeof(quint16));

    socket_->write(block);

    return true;
}

bool TcpChatConnection::isAlive() {
	return socket_->isOpen() && socket_->isValid() && 
		socket_->isWritable() && socket_->isReadable();
}

std::string TcpChatConnection::getIdent() {
	auto str =  socket_->peerAddress().toString() + ":" + QString::number(socket_->peerPort());
	return str.toStdString();
}

std::shared_ptr<QTcpSocket> TcpChatConnection::socket() {
    return socket_;
}

}
