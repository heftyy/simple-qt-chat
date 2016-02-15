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
    start(port, ipAddress);

    connect(tcpServer_, SIGNAL(newConnection()),
            this, SLOT(connectionEstablished()));
}

void TcpChatServer::handleUntypedMessage(const QString& serializedMessage) {
    auto connection = static_cast<TcpChatConnection*>(sender());

    auto deserializer = MessageDeserializer(serializedMessage.toStdString());

    receiveUntypedMessage(deserializer, connection);
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
}

void TcpChatServer::connectionLost() {       
    auto connection = static_cast<TcpChatConnection*>(sender());

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
    chateeLeft(chatee);

    connection->deleteLater();
}

void TcpChatServer::start(quint16 port, const QHostAddress& ipAddress) {
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

} // SimpleChat namespace
