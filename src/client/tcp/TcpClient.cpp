#include <iostream>

#include <QtNetwork>
#include <QtWidgets>

#include <User.pb.h>
#include <ChatMessage.pb.h>
#include <NetworkMessage.pb.h>
#include <communication/Message.h>
#include <communication/MessageDeserializer.h>

#include "TcpClient.h"
#include "TcpChatConnection.h"

namespace SimpleChat {

void TcpClient::showChatWindow() {

}

void TcpClient::requestUserList() {

}

void TcpClient::sendCommand(std::string command) {

}

void TcpClient::sendMessage(std::string text, std::string target) {
    std::unique_ptr<ChatMessage> chatMessage = std::make_unique<ChatMessage>();
    chatMessage->set_text("a");
    chatMessage->set_timestamp(66666);

    serverConnection_->sendMessage(std::make_unique<Message<ChatMessage>>(
            std::move(chatMessage), CHAT_MESSAGE));
}

void TcpClient::serverDialog() {
    std::cout << "server address:\n";
    std::string address;
    std::cin >> address;

    std::cout << "server port:\n";
    std::cin >> serverPort_;

    serverAddress_ = QHostAddress(QString::fromStdString(address));
}

void TcpClient::nameDialog() {
    std::cout << "name:\n";
    std::string name;
    std::cin >> name;

    clientName_ = name;
}

void TcpClient::connect() {
    if(!clientName_.empty() && !serverAddress_.isNull()) {
        auto socket = std::make_shared<QTcpSocket>();

//        connect(socket.get(), SIGNAL(displayError(QAbstractSocket::SocketError)),

        serverConnection_ = std::make_shared<TcpChatConnection>(socket);

        socket->connect(socket.get(), &QTcpSocket::readyRead,
                        socket.get(), [this] { dataReceived(); });

        socket->connectToHost(serverAddress_, serverPort_);
    }
}

void TcpClient::join() {
    auto joinRequest = std::make_unique<UserJoinRequest>();

    joinRequest->set_name(clientName_);

    serverConnection_->sendMessage(std::make_unique<Message<UserJoinRequest>>(
            std::move(joinRequest), USER_JOIN_REQUEST));
}

void TcpClient::dataReceived() {
    std::cout << "DATA RECEIVED\n";

    QDataStream inStream(serverConnection_->socket().get());
    inStream.setVersion(QDataStream::Qt_5_5);

    if (serverConnection_->blockSize == 0) {
        if (serverConnection_->socket()->bytesAvailable() < static_cast<int>(sizeof(quint16)))
            return;

        inStream >> serverConnection_->blockSize;
    }

    if (serverConnection_->socket()->bytesAvailable() < serverConnection_->blockSize)
        return;

    // full message received, reset blockSize to 0
    serverConnection_->blockSize = 0;

    QString serializedMessage;
    inStream >> serializedMessage;

    std::cout << serializedMessage.toStdString() << std::endl;

    MessageDeserializer deserializer(serializedMessage.toStdString());
    handleUntypedMessage(deserializer, serverConnection_);
}

void TcpClient::handleUntypedMessage(const MessageDeserializer& deserializer,
                                     const std::shared_ptr<ChatConnection>& connection) {
    if(connection == nullptr || !connection->isAlive()) {
        std::cerr << "chat connection is invalid" << std::endl;
        return;
    }

    if (!deserializer.isInitialized())
        return;

    if (deserializer.type() == USER_JOIN_RESPONSE) {
        handleMessage(deserializer.getMessage<UserJoinResponse>());
    }
}

void TcpClient::displayError(QAbstractSocket::SocketError socketError) {
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
    std::cout << "Fortune Client\n" <<
                                 "The host was not found. Please check the "
                                            "host name and port settings.\n";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        std::cout << "Fortune Client\n"
                                 << "The connection was refused by the peer. "
                                            "Make sure the fortune server is running, "
                                            "and check that the host name and port "
                                            "settings are correct.\n";
        break;
    default:
        std::cout <<"Fortune Client\n"
                << "The following error occurred: "
        << serverConnection_->socket()->errorString().toStdString() << std::endl;
    }
}

QHostAddress TcpClient::getServerAddress() {
    return serverAddress_;
}

void TcpClient::handleMessage(std::unique_ptr<UserJoinResponse> joinRequest) {
    std::cout << "joined succesfully " <<
            joinRequest->user().name() << " " <<
            joinRequest->user().id() <<
            std::endl;
}

void TcpClient::handleMessage(std::unique_ptr<UserListResponse> listRequest) {

}

} // SimpleChat namespace