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

#include "TcpServer.h"

namespace SimpleChat {

TcpServer::TcpServer(const std::string& password) :
        tcpServer_(nullptr),
        chatroom_(std::make_shared<Chatroom>()),
        password_(password) {

}

void TcpServer::listen(quint16 port, QHostAddress ipAddress) {
    openSession(port, ipAddress);

    connect(tcpServer_.get(), SIGNAL(newConnection()),
            this, SLOT(connectionEstablished()));
}

QHostAddress TcpServer::getAddress() const {
    auto ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (auto ipAddress : ipAddressesList) {
        if (ipAddress != QHostAddress::LocalHost && ipAddress.toIPv4Address()) {
            return ipAddress;
        }
    }

    return QHostAddress::LocalHost;
}

void TcpServer::connectionEstablished() {
    auto clientConnection = tcpServer_->nextPendingConnection();

    qDebug() << "SERVER: " << "new connection from" << clientConnection->peerAddress().toString();

    auto connection = std::make_shared<TcpChatConnection>(
            std::shared_ptr<QTcpSocket>(clientConnection));

    connections_.emplace(std::make_pair(connection->getIdent(), connection));

    connection->connect(
            connection.get(),
            SIGNAL(dataReceived(QString, std::string)),
            this,
            SLOT(handleUntypedMessage(QString, std::string))
    );

    connection->connect(
            connection.get(),
            SIGNAL(disconnectSignal(std::string)),
            this,
            SLOT(connectionLost(std::string))
    );

    connection->init();
}

void TcpServer::connectionLost(std::string ident) {
    auto connection = connections_[ident];

    if (connection == nullptr) {
        qCritical() << "disconnected called with empty connection";
        return;
    }    

    if (connection->chatee() == nullptr) {
        qCritical() << "disconnected called with empty chatee";
        return;
    }    

    // get a shared_ptr to chatee so we can send a message after it's removed from chatroom
    // connection stores a weak_ptr
    auto chatee = connection->chatee();

    bool success;
    std::string message;

    std::tie(success, message) = chatroom_->chateeLeft(connection->chatee()->user().name());

    if (success) {
        qDebug() << "SERVER:" << chatee->user().DebugString().c_str() << "from" <<
            connection->socket()->peerAddress().toString() << "left";

        auto change = std::make_unique<UserChange>();
        change->set_action(UserAction::LEFT);
        change->mutable_user()->CopyFrom(chatee->user());

        chatroom_->propagateMessage(std::make_unique<Message<UserChange>>(
            std::move(change), USER_CHANGE));
    }
    else
        qCritical() << "removing the chatee failed";
}

void TcpServer::handleUntypedMessage(QString serializedData,
                                     std::string ident) {

    auto deserializer = MessageDeserializer(serializedData.toStdString());
    auto connection = connections_[ident];

    if (connection == nullptr || !connection->isAlive()) {
        qCritical() << "chat connection is invalid";
        return;
    }

    if (!deserializer.isInitialized())
        return;

    if (deserializer.type() == USER_JOIN_REQUEST) {
        handleMessage(deserializer.getMessage<UserJoinRequest>(), connection);
        return;
    }

    if (connection->chatee() == nullptr) {
        qCritical() << "chatee not found for connection from " << connection->getIdent().c_str();
        return;
    }

    if (deserializer.type() == USER_LIST_REQUEST) {
        handleMessage(deserializer.getMessage<UserListRequest>(), connection->chatee());
    }
    else if (deserializer.type() == USER_CHANGE) {
        handleMessage(deserializer.getMessage<UserChange>(), connection->chatee());
    }
    else if (deserializer.type() == CHAT_MESSAGE) {
        handleMessage(deserializer.getMessage<ChatMessage>(), connection->chatee());
    }
    else if (deserializer.type() == CHAT_AUTHORIZE) {
        handleMessage(deserializer.getMessage<ChatAuthorize>(), connection->chatee());
    }
    else if (deserializer.type() == CHAT_COMMAND) {
        handleMessage(deserializer.getMessage<ChatCommand>(), connection->chatee());
    }
}

void TcpServer::openSession(quint16 port, QHostAddress ipAddress) {
    if (ipAddress == QHostAddress::LocalHost)
        ipAddress = getAddress();

    tcpServer_ = std::make_shared<QTcpServer>(this);
    if (!tcpServer_->listen(ipAddress, port)) {
        qCritical() << "opening listening port failed";
        return;
    }

    qDebug() << "SERVER: " <<
        "The server is running on\nIP: " <<
        ipAddress.toString() <<
        "\nport: " << tcpServer_->serverPort();
}

void TcpServer::handleMessage(std::unique_ptr<UserJoinRequest> joinRequest,
                              const std::shared_ptr<ChatConnection>& connection) {
    bool success;
    std::string message;
    std::shared_ptr<Chatee> chatee;
    std::tie(success, message, chatee) = chatroom_->chateeJoined(joinRequest->name(), connection);

    auto response = std::make_unique<UserJoinResponse>();
    response->set_success(success);
    response->set_message(message);
    response->set_motd(chatroom_->motd());

    if (success) {
        response->mutable_user()->CopyFrom(chatee->user());

        auto userChange = std::make_unique<UserChange>();
        userChange->set_action(UserAction::JOINED);
        userChange->mutable_user()->CopyFrom(chatee->user());
        chatroom_->propagateMessage(std::make_unique<Message<UserChange>>(
                std::move(userChange), USER_CHANGE));

        qDebug() << "SERVER:" << joinRequest->DebugString().c_str();
    }

    chatee->sendMessage(std::make_unique<Message<UserJoinResponse>>(
            std::move(response), USER_JOIN_RESPONSE));
}

void TcpServer::handleMessage(std::unique_ptr<UserListRequest> listRequest,
                              const std::shared_ptr<Chatee>& sender) {
    auto response = std::make_unique<UserListResponse>();

    auto const& map = chatroom_->map();
    for (auto const& pair : map) {
        response->mutable_users()->Add()->CopyFrom(
                pair.second->user());
    }

    sender->sendMessage(std::make_unique<Message<UserListResponse>>(
            std::move(response), USER_LIST_RESPONSE));
}

void TcpServer::handleMessage(std::unique_ptr<UserChange> change,
                              const std::shared_ptr<Chatee>& sender) {
    if (change->has_presence())
        sender->user().set_presence(change->presence());
    if (change->has_status())
        sender->user().set_status(change->status());

    chatroom_->propagateMessage(std::make_unique<Message<UserChange>>(
            std::move(change), USER_CHANGE));
}

void TcpServer::handleMessage(std::unique_ptr<ChatMessage> chatMessage, const std::shared_ptr<Chatee>& sender) {
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    chatMessage->set_timestamp(123421521);

    if (chatMessage->has_target()) { // send a private message
        auto targetChatee = chatroom_->getChatee(chatMessage->target().user_name());
        if (targetChatee == nullptr) { // let the sender know that target doesn't exist
            auto message = "user with name " + chatMessage->target().user_name() + " doesn't exit";
            qCritical() << message.c_str();

            sender->sendResponse(false, message);
        }
        else {
            // tell sender - message from sender to target was sent
            sender->sendChatMessage(chatMessage->text(),
                                    sender->user().name(),
                                    chatMessage->target().user_name());

            // tell target - message from sender was received
            targetChatee->sendChatMessage(chatMessage->text(),
                                          sender->user().name());
        }
    }
    else { // send a public message
        qDebug() << "SERVER: " << chatMessage->DebugString().c_str();

        chatroom_->propagateMessage(std::make_unique<Message<ChatMessage>>(
                std::move(chatMessage), CHAT_MESSAGE));
    }
}

void TcpServer::handleMessage(std::unique_ptr<ChatAuthorize> chatAuthorize, const std::shared_ptr<Chatee>& sender) {
    if (chatAuthorize->password() == password_) {
        sender->setAuthorized(true);
        sender->sendResponse(true, "auth successful");
    }
    else
        sender->sendResponse(false, "auth failure");
}

void TcpServer::handleMessage(std::unique_ptr<ChatCommand> chatCommand, const std::shared_ptr<Chatee>& sender) {
    if (sender->authorized()) {
        if (chatCommand->type() == CommandType::MUTE) {
            auto targetChatee = chatroom_->getChatee(chatCommand->arguments(0));
            if (targetChatee == nullptr)
                sender->sendResponse(false, "user " + chatCommand->arguments(0) + " doesn't exist");
            targetChatee->mute();
        }
        else if (chatCommand->type() == CommandType::KICK) {
            auto targetChatee = chatroom_->getChatee(chatCommand->arguments(0));
            if (targetChatee == nullptr)
                sender->sendResponse(false, "user " + chatCommand->arguments(0) + " doesn't exist");
            targetChatee->kick();
        }
        else if (chatCommand->type() == CommandType::MOTD) {
            chatroom_->setMotd(chatCommand->arguments(0));
        }
    }
    else
        sender->sendResponse(false, "you are not authorized to use this command");
}

} //SimpleChat namespace
