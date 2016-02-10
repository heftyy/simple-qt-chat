#include <QtNetwork>

#include <NetworkMessage.pb.h>
#include <User.pb.h>
#include <ChatMessage.pb.h>

#include <chat/Chatroom.h>
#include <chat/Chatee.h>
#include <communication/Message.h>
#include <communication/MessageDeserializer.h>

#include "TcpServer.h"
#include "TcpChatConnection.h"

namespace SimpleChat {

TcpServer::TcpServer(const std::string& password) :
        tcpServer_(nullptr),
        chatroom_(std::make_shared<Chatroom>()),
        password_(password) {

}

void TcpServer::listen(quint16 port, QHostAddress ipAddress) {
    openSession(port, ipAddress);

    connect(tcpServer_.get(), &QTcpServer::newConnection,
            tcpServer_.get(), [this] { connectionEstablished(); });
}

void TcpServer::handleUntypedMessage(const MessageDeserializer& deserializer,
                                     const std::shared_ptr<ChatConnection>& connection) {
    if(connection == nullptr || !connection->isAlive()) {
        std::cerr << "chat connection is invalid" << std::endl;
        return;
    }

    if (!deserializer.isInitialized())
        return;

    if (deserializer.type() == USER_JOIN_REQUEST) {
        handleMessage(deserializer.getMessage<UserJoinRequest>(), connection);
        return;
    }

    if(connection->chatee() == nullptr) {
        std::cerr << "chatee not found for connection from " << connection->getIdent() << std::endl;
        return;
    }

    if(deserializer.type() == USER_LIST_REQUEST) {
        handleMessage(deserializer.getMessage<UserListRequest>(), connection->chatee());
    }
    else if (deserializer.type() == USER_CHANGE) {
        handleMessage(deserializer.getMessage<UserChange>(), connection->chatee());
    }
    else if(deserializer.type() == CHAT_MESSAGE) {
        handleMessage(deserializer.getMessage<ChatMessage>(), connection->chatee());
    }
    else if(deserializer.type() == CHAT_AUTHORIZE) {
        handleMessage(deserializer.getMessage<ChatAuthorize>(), connection->chatee());
    }
    else if(deserializer.type() == CHAT_COMMAND) {
        handleMessage(deserializer.getMessage<ChatCommand>(), connection->chatee());
    }
}

QHostAddress TcpServer::getAddress() {
    auto ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (auto ipAddress : ipAddressesList) {
        if (ipAddress != QHostAddress::LocalHost && ipAddress.toIPv4Address()) {
            return ipAddress;
        }
    }

    return QHostAddress::LocalHost;
}

void TcpServer::connectionEstablished() const {
    auto clientConnection = tcpServer_->nextPendingConnection();

    std::cout << "new connection from " << clientConnection->peerAddress().toString().toStdString() << std::endl;

    auto connection = std::make_shared<TcpChatConnection>(
        std::shared_ptr<QTcpSocket>(clientConnection));

    clientConnection->
            connect(clientConnection, &QTcpSocket::disconnected,
                    clientConnection, [this, connection] { disconnected(connection); });

    clientConnection->
            connect(clientConnection, &QTcpSocket::readyRead,
                    clientConnection, [this, connection] { dataReceived(connection); });
}

void TcpServer::disconnected(const std::shared_ptr<TcpChatConnection>& connection) {
    if(connection == nullptr) {
        std::cerr << "disconnected called with empty connection" << std::endl;
        return;
    }

    connection->socket()->disconnectFromHost();

    if(connection->chatee() == nullptr) {
        std::cerr << "disconnected called with empty chatee" << std::endl;
        return;
    }

    // get a shared_ptr to chatee so we can send a message after it's removed from chatroom
    // connection stores a weak_ptr
    auto chatee = connection->chatee();

    bool success;
    std::string message;

    std::tie(success, message) = chatroom_->chateeLeft(connection->chatee()->user().name());

    if(success) {
        std::cout << "user " << chatee->user().name() << " from " <<
                connection->socket()->peerAddress().toString().toStdString() << " left" << std::endl;

        auto change = std::make_unique<UserChange>();
        change->set_action(UserAction::LEFT);
        change->mutable_user()->CopyFrom(chatee->user());

        chatroom_->propagateMessage(std::make_unique<Message<UserChange>>(
                std::move(change), USER_CHANGE));
    }
    else
        std::cerr << "removing the chatee failed" << std::endl;
}

void TcpServer::dataReceived(const std::shared_ptr<TcpChatConnection>& connection) {
    QDataStream inStream(connection->socket().get());
    inStream.setVersion(QDataStream::Qt_5_5);

    if (connection->blockSize == 0) {
        if (connection->socket()->bytesAvailable() < static_cast<int>(sizeof(quint16)))
            return;

        inStream >> connection->blockSize;
    }

    if (connection->socket()->bytesAvailable() < connection->blockSize)
        return;

    // full message received, reset blockSize to 0
    connection->blockSize = 0;

    QString serializedMessage;
    inStream >> serializedMessage;

    MessageDeserializer deserializer(serializedMessage.toStdString());
    handleUntypedMessage(deserializer, connection);
}

void TcpServer::openSession(quint16 port, QHostAddress ipAddress) {
    if (ipAddress == QHostAddress::LocalHost)
        ipAddress = getAddress();

    tcpServer_ = std::make_shared<QTcpServer>(this);
    if (!tcpServer_->listen(ipAddress, port)) {
        std::cerr << "opening listening port failed" << std::endl;
        return;
    }

    std::cout <<
    "The server is running on\n IP: " <<
    ipAddress.toString().toStdString() <<
    "\nport: " << tcpServer_->serverPort() <<
    std::endl;
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
    }

    chatee->sendMessage(std::make_unique<Message<UserJoinResponse>>(
            std::move(response), USER_JOIN_RESPONSE));

    std::cout << "user " << joinRequest->name() << " joined" << std::endl;
}

void TcpServer::handleMessage(std::unique_ptr<UserListRequest> listRequest,
                              const std::shared_ptr<Chatee>& sender) {
    auto response = std::make_unique<UserListResponse>();

    auto const& map = chatroom_->map();
    for(auto const& pair : map) {
        response->mutable_users()->Add()->CopyFrom(
                pair.second->user());
    }

    sender->sendMessage(std::make_unique<Message<UserListResponse>>(
            std::move(response), USER_LIST_RESPONSE));
}

void TcpServer::handleMessage(std::unique_ptr<UserChange> change,
                              const std::shared_ptr<Chatee>& sender) {
    if(change->has_presence())
        sender->user().set_presence(change->presence());
    if(change->has_status())
        sender->user().set_status(change->status());

    chatroom_->propagateMessage(std::make_unique<Message<UserChange>>(
            std::move(change), USER_CHANGE));
}

void TcpServer::handleMessage(std::unique_ptr<ChatMessage> chatMessage, const std::shared_ptr<Chatee>& sender) {
    if(chatMessage->has_target()) { // send a private message
        auto targetChatee = chatroom_->getChatee(chatMessage->target().user_name());
        if(targetChatee == nullptr) { // let the sender know that target doesn't exist
            std::string message = "user with name " + chatMessage->target().user_name() + " doesn't exit";
            std::cerr << message << std::endl;

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
        std::cout << sender->user().name() << ": " << chatMessage->text() << std::endl;

        chatroom_->propagateMessage(std::make_unique<Message<ChatMessage>>(
                std::move(chatMessage), CHAT_MESSAGE));
    }
}

void TcpServer::handleMessage(std::unique_ptr<ChatAuthorize> chatAuthorize, const std::shared_ptr<Chatee>& sender) {
    if(chatAuthorize->password() == password_) {
        sender->setAuthorized(true);
        sender->sendResponse(true, "auth successful");
    }
    else
        sender->sendResponse(false, "auth failure");
}

void TcpServer::handleMessage(std::unique_ptr<ChatCommand> chatCommand, const std::shared_ptr<Chatee>& sender) {
    if(sender->authorized()) {
        if(chatCommand->type() == CommandType::MUTE) {
            auto targetChatee = chatroom_->getChatee(chatCommand->arguments(0));
            if(targetChatee == nullptr)
                sender->sendResponse(false, "user " + chatCommand->arguments(0) + " doesn't exist");
            targetChatee->mute();
        }
        else if(chatCommand->type() == CommandType::KICK) {
            auto targetChatee = chatroom_->getChatee(chatCommand->arguments(0));
            if(targetChatee == nullptr)
                sender->sendResponse(false, "user " + chatCommand->arguments(0) + " doesn't exist");
            targetChatee->kick();
        }
        else if(chatCommand->type() == CommandType::MOTD) {
            chatroom_->setMotd(chatCommand->arguments(0));
        }
    }
    else
        sender->sendResponse(false, "you are not authorized to use this command");
}

} //SimpleChat namespace