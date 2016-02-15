#include <iostream>
#include <QtNetwork>

#include <communication/AbstractMessage.h>
#include <communication/MessageSerializer.h>
#include <communication/MessageDeserializer.h>

#include "TcpChatConnection.h"

namespace SimpleChat {

TcpChatConnection::TcpChatConnection(QTcpSocket* socket, QObject* parent) :
        QObject(parent),
        blockSize_(0),
        socket_(socket) {

}

TcpChatConnection::~TcpChatConnection() {
    socket_->deleteLater();
    qDebug() << "connection deleted";
}

void TcpChatConnection::init() const {
    socket_->connect(socket_, 
                     SIGNAL(disconnected()),
                     this, 
                     SLOT(disconnected()));

    socket_->connect(socket_, 
                     SIGNAL(readyRead()),
                     this, 
                     SLOT(readyRead()));
}

bool TcpChatConnection::sendMessage(std::unique_ptr<AbstractMessage> message) {
    if (!isAlive())
        return false;

    MessageSerializer serializer(std::move(message));

    bool success;
    std::string result;
    std::tie(success, result) = serializer.serialize();

    if (!success) {
        qCritical() << "serialization failed ";
        return false;
    }

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_5);

    out << static_cast<quint16>(0);
    out << QString::fromStdString(result);
    out.device()->seek(0);
    out << static_cast<quint16>(block.size() - sizeof(quint16));

    qDebug() << "sending block size: " << block.size();

    socket_->write(block);
    return true;
}

bool TcpChatConnection::isAlive() const {
    return socket_->state() == QAbstractSocket::ConnectedState &&
            socket_->isOpen() && socket_->isValid() &&
            socket_->isWritable() && socket_->isReadable();
}

std::string TcpChatConnection::getIdent() const {
    auto str = socket_->peerAddress().toString() + ":" + QString::number(socket_->peerPort());
    return str.toStdString();
}

void TcpChatConnection::disconnectFromHost() {
    socket_->disconnectFromHost();
}

QTcpSocket* TcpChatConnection::socket() const {
    return socket_;
}

void TcpChatConnection::setChatee(const std::shared_ptr<Chatee>& chatee) {
    chatee_ = chatee;
}

std::shared_ptr<Chatee> TcpChatConnection::chatee() const {
    if(!chatee_.expired())
        return chatee_.lock();
    return nullptr;
}

void TcpChatConnection::readyRead() {
    qDebug() << getIdent().c_str() << " readyRead";

    QDataStream inStream(socket());
    inStream.setVersion(QDataStream::Qt_5_5);

    while(socket()->bytesAvailable() > 0) {
        if (blockSize_ == 0) {
            if (socket()->bytesAvailable() < sizeof(quint16))
                return;

            inStream >> blockSize_;
            qDebug() << "received block size: " << blockSize_;
        }

        if (socket()->bytesAvailable() < blockSize_)
            return;

        //! full message received, reset blockSize to 0
        blockSize_ = 0;

        QString serializedMessage;
        inStream >> serializedMessage;

        emit dataReceivedSignal(serializedMessage);
    }
}

void TcpChatConnection::disconnected() {
    emit disconnectSignal();
}

} // SimpleChat namespace
