#include <iostream>

#include <QtNetwork>

#include <NetworkMessage.pb.h>
#include <User.pb.h>
#include <ChatMessage.pb.h>

#include <chat/Chatroom.h>
#include <chat/Chatee.h>
#include <communication/Message.h>
#include <communication/MessageDeserializer.h>

#include "../chat/TcpChatConnection.h"

#include "TcpChatServer.h"

namespace SimpleChat {

TcpChatServer::TcpChatServer(const std::string& password, QObject* parent) :
        QObject(parent),
        ChatServer(password),
        tcpServer_(nullptr) {

}

TcpChatServer::~TcpChatServer() {
    if(tcpServer_ != nullptr)
        tcpServer_->deleteLater();
}

void TcpChatServer::listen(quint16 port, const QHostAddress& ipAddress) {
    openSession(port, ipAddress);

    connect(tcpServer_, SIGNAL(newConnection()),
            this, SLOT(connectionEstablished()));
}

void TcpChatServer::handleUntypedMessage(const QString& serializedMessage) {
    auto ident = static_cast<TcpChatConnection*>(sender())->getIdent();

    auto deserializer = MessageDeserializer(serializedMessage.toStdString());
    auto connection = connections_[ident];

    ChatServer::handleUntypedMessage(deserializer, connection);
}

void TcpChatServer::connectionEstablished() {
    auto socket = tcpServer_->nextPendingConnection();
    auto connection = new TcpChatConnection(socket, this);

    qDebug() << "SERVER: " << "new connection from" << socket->peerAddress().toString();    

    connection->init();

    connection->connect(connection,
                        SIGNAL(dataReceivedSignal(QString)),
                        this,
                        SLOT(handleUntypedMessage(QString))
    );

    connection->connect(connection,
                        SIGNAL(disconnectSignal()),
                        this,
                        SLOT(connectionLost())
    );

    connections_.emplace(std::make_pair(connection->getIdent(), connection));
}

void TcpChatServer::connectionLost() {       
    auto connection = static_cast<TcpChatConnection*>(sender());
    connections_.erase(connection->getIdent());

    if (connection == nullptr) {
        qCritical() << "disconnected called with empty connection";
        return;
    }
    
    if (connection->chatee() == nullptr) {
        qCritical() << "disconnected called with empty chatee";
        connection->deleteLater();
        return;
    }

    /*
    get a shared_ptr to chatee from connection so we can send a 
    message after it's removed from chatroom

    connection stores a weak_ptr of chatee
    */
    
    auto chatee = connection->chatee();

    bool success;
    std::string message;

    std::tie(success, message) = chatroom_->chateeLeft(connection->chatee()->user().name());

    if (success) {
        qDebug() << "SERVER:" << chatee->user().DebugString().c_str() << "from" <<
            connection->socket()->peerAddress().toString() << "left";

        auto userChange = std::make_unique<UserChange>();
        userChange->set_action(UserAction::LEFT);
        userChange->mutable_user()->CopyFrom(chatee->user());

        chatroom_->propagateMessage(std::make_unique<Message<UserChange>>(
            std::move(userChange), USER_CHANGE));
    }
    else
        qCritical() << "removing the chatee failed";

    connection->deleteLater();
    QCoreApplication::exit();
}

void TcpChatServer::openSession(quint16 port, const QHostAddress& ipAddress) {
    tcpServer_ = new QTcpServer(this);
    if (!tcpServer_->listen(ipAddress, port)) {
        qCritical() << "opening listening port failed";
        return;
    }

    qDebug() << "SERVER: " <<
        "The server is running on\nIP: " <<
        ipAddress.toString() <<
        "\nport: " << tcpServer_->serverPort();
}

} //SimpleChat namespace
