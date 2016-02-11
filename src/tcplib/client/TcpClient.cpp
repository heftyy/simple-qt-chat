#include <iostream>

#include <QtNetwork>

#include <User.pb.h>
#include <ChatMessage.pb.h>
#include <NetworkMessage.pb.h>
#include <communication/Message.h>
#include <communication/MessageDeserializer.h>
#include <chat/TcpChatConnection.h>

#include "TcpClient.h"

namespace SimpleChat {

void TcpClient::showChatWindow() {

}

void TcpClient::requestUserList() {

}

void TcpClient::sendCommand(std::string command) {

}

void TcpClient::sendMessage(std::string text, std::string target) {
    auto chatMessage = std::make_unique<ChatMessage>();
    chatMessage->set_text(text);

    auto result = serverConnection_->sendMessage(std::make_unique<Message<ChatMessage>>(
            std::move(chatMessage), CHAT_MESSAGE));

    if (!result)
        qCritical() << "sending a message failed";
}

void TcpClient::serverDialog() {
    qDebug() << "server address:\n";
    std::string address;
    std::cin >> address;

    qDebug() << "server port:\n";
    std::cin >> serverPort_;

    serverAddress_ = QHostAddress(QString::fromStdString(address));
}

void TcpClient::nameDialog() {
    qDebug() << "name:\n";
    std::string name;
    std::cin >> name;

    clientName_ = name;
}

void TcpClient::connectToHost() {
    if(!clientName_.empty() && !serverAddress_.isNull()) {
        auto socket = std::make_shared<QTcpSocket>();

        serverConnection_ = std::make_shared<TcpChatConnection>(socket);
        serverConnection_->init();

        serverConnection_->connect(
                serverConnection_.get(),
                SIGNAL(dataReceived(QString, std::string)),
                this,
                SLOT(handleUntypedMessage(QString, std::string))
        );

        serverConnection_->connect(
                serverConnection_.get(),
                SIGNAL(disconnectSignal(std::string)),
                this,
                SLOT(connectionLost(std::string))
        );

        socket->connectToHost(serverAddress_, serverPort_);
        socket->waitForConnected();
    }
}

void TcpClient::join() {
    auto joinRequest = std::make_unique<UserJoinRequest>();

    joinRequest->set_name(clientName_);

    serverConnection_->sendMessage(std::make_unique<Message<UserJoinRequest>>(
            std::move(joinRequest), USER_JOIN_REQUEST));
}

void TcpClient::displayError(QAbstractSocket::SocketError socketError) const {
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug() << "Fortune Client\n" <<
            "The host was not found. Please check the "
            "host name and port settings.";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qDebug() << "Fortune Client\n"
            << "The connection was refused by the peer. "
            "Make sure the fortune server is running, "
            "and check that the host name and port "
            "settings are correct.";
        break;
    default:
        qDebug() << "Fortune Client\n"
            << "The following error occurred: "
            << serverConnection_->socket()->errorString();
    }
}

QHostAddress TcpClient::getServerAddress() {
    return serverAddress_;
}

void TcpClient::handleMessage(std::unique_ptr<UserJoinResponse> joinResponse) {
    qDebug() << "joined succesfully: " <<
        joinResponse->DebugString().c_str();;

    sendMessage("brbrbrbr1", "notarget");
    sendMessage("brbrbrbr2", "notarget");
    sendMessage("brbrbrbr3", "notarget");
}

void TcpClient::handleMessage(std::unique_ptr<UserListResponse> listRequest) {

}

void TcpClient::handleMessage(std::unique_ptr<UserChange> userChange) {
    qDebug() << "user change " <<
        userChange->DebugString().c_str();
}

void TcpClient::handleMessage(std::unique_ptr<ChatMessage> chatMessage) {
    qDebug() << "chat messsage " <<
        chatMessage->DebugString().c_str();
}

void TcpClient::handleUntypedMessage(QString serializedData,
                                     std::string ident) {

    auto deserializer = MessageDeserializer(serializedData.toStdString());

    if(serverConnection_ == nullptr || !serverConnection_->isAlive() ||
       serverConnection_->getIdent() != ident) {
        qCritical() << "chat connection is invalid";
        return;
    }

    if (!deserializer.isInitialized())
        return;

    if (deserializer.type() == USER_JOIN_RESPONSE) {
        handleMessage(deserializer.getMessage<UserJoinResponse>());
    }
    else if(deserializer.type() == USER_CHANGE) {
        handleMessage(deserializer.getMessage<UserChange>());
    }
    else if(deserializer.type() == CHAT_MESSAGE) {
        handleMessage(deserializer.getMessage<ChatMessage>());
    }
}

void TcpClient::connectionLost(std::string ident) {
    qDebug() << "disconnected\n";
}

void TcpClient::disconnectFromHost() {
    serverConnection_->socket()->disconnectFromHost();
    serverConnection_->socket()->waitForDisconnected();


    serverConnection_.reset();
}

} // SimpleChat namespace
