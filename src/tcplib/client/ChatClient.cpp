#include <iostream>

#include <QtNetwork>

#include <User.pb.h>
#include <ChatMessage.pb.h>
#include <NetworkMessage.pb.h>
#include <communication/Message.h>
#include <communication/MessageDeserializer.h>
#include <chat/TcpChatConnection.h>
#include <chat/Chatroom.h>
#include <chat/Chatee.h>
#include <chat/commands/CommandParser.h>

#include "ChatClient.h"

namespace SimpleChat {

ChatClient::ChatClient() :
        chatroom_(new Chatroom),
        serverAddress_(QHostAddress::LocalHost),
        serverConnection_(nullptr),
        serverPort_(0){

}

void ChatClient::sendCommand(const std::string& command) {
    CommandParser commandParser(command);

    auto message = commandParser.chatCommand(chatroom_->getTarget(clientName_));
    if(message) {
        auto result = serverConnection_->sendMessage(
                std::make_unique<Message<ChatCommand>>(
                        std::move(message), CHAT_COMMAND));

        if (!result)
            qCritical() << "sending a command failed";
    }
}

void ChatClient::sendMessage(const std::string& text, const std::string& target) {
    auto chatMessage = std::make_unique<ChatMessage>();
    chatMessage->set_text(text);

    auto result = serverConnection_->sendMessage(
            std::make_unique<Message<ChatMessage>>(std::move(chatMessage), CHAT_MESSAGE));

    if (!result)
        qCritical() << "sending a message failed";
}

bool ChatClient::login(const QString& address, quint16 port, const QString& name) {
    serverAddress_ = QHostAddress(address);
    serverPort_ = port;
    clientName_ = name.toStdString();

    return connectToServer();
}

void ChatClient::logout() {
    serverConnection_->socket()->disconnectFromHost();
}

void ChatClient::handleUntypedMessage(const QString& serializedData) {

    auto deserializer = MessageDeserializer(serializedData.toStdString());

    if (serverConnection_ == nullptr || !serverConnection_->isAlive()) {
        qCritical() << "chat connection is invalid";
        return;
    }

    if (!deserializer.isInitialized())
        return;

    if (deserializer.type() == USER_JOIN_RESPONSE) {
        handleMessage(deserializer.getMessage<UserJoinResponse>());
    }
    else if (deserializer.type() == USER_LIST_RESPONSE) {
        handleMessage(deserializer.getMessage<UserListResponse>());
    }
    else if (deserializer.type() == USER_CHANGE) {
        handleMessage(deserializer.getMessage<UserChange>());
    }
    else if (deserializer.type() == CHAT_MESSAGE) {
        handleMessage(deserializer.getMessage<ChatMessage>());
    }
    else if(deserializer.type() == CHATROOM_CHANGE) {
        handleMessage(deserializer.getMessage<ChatroomChange>());
    }
    else if(deserializer.type() == GENERIC_CHAT_RESPONSE) {
        handleMessage(deserializer.getMessage<GenericChatResponse>());
    }
}

void ChatClient::connectionLost() {
    qDebug() << "disconnected\n";
}

void ChatClient::displayError(QAbstractSocket::SocketError socketError) const {
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

bool ChatClient::connectToServer() {
    if (clientName_.empty() || serverAddress_.isNull() || serverPort_ <= 0) {
        return false;
    }

    auto socket = std::make_shared<QTcpSocket>();
    serverConnection_ = std::make_shared<TcpChatConnection>(socket);
    serverConnection_->init();

    serverConnection_->connect(
            serverConnection_.get(),
            SIGNAL(dataReceivedSignal(QString)),
            this,
            SLOT(handleUntypedMessage(QString)));

    serverConnection_->connect(
            serverConnection_.get(),
            SIGNAL(disconnectSignal()),
            this,
            SLOT(connectionLost()));

    serverConnection_->connect(
            serverConnection_.get(),
            SIGNAL(error(QAbstractSocket::SocketError)),
            this,
            SLOT(displayError(QAbstractSocket::SocketError)));

    socket->connectToHost(serverAddress_, serverPort_);
    if(socket->waitForConnected()) {
        join();
        return true;
    }
    return false;
}

void ChatClient::handleMessage(std::unique_ptr<UserJoinResponse> joinResponse) {
    qDebug() << "joined successfully: " <<
    joinResponse->DebugString().c_str();

    if(joinResponse->success()) {
        requestUserList();
        chatroom_->chateeJoined(joinResponse->user(), serverConnection_);

        emit chatMotdSignal(
                format(joinResponse->motd()));
        emit chatInfoSignal(
                format(joinResponse->user(), "joined"));
    }
    else {
        emit chatInfoSignal(
                format(joinResponse->message()));
    }
}

void ChatClient::handleMessage(std::unique_ptr<UserListResponse> listResponse) {
    for(auto const& user : listResponse->users()) {
        chatroom_->chateeJoined(user, serverConnection_);
    }

    emit refreshChateeList();
}

void ChatClient::handleMessage(std::unique_ptr<UserChange> userChange) {
    qDebug() << "user change " <<
    userChange->DebugString().c_str();

    auto chatee = chatroom_->getChatee(userChange->user().name());
    if(chatee == nullptr)
        return;

    if(userChange->has_presence()) {
        chatee->user().set_presence(userChange->presence());
        emit chatInfoSignal(
                format(userChange->user(), "is now " + UserPresence_Name(userChange->presence()))
        );
    }
    if(userChange->has_status()) {
        chatee->user().set_status(userChange->status());
        if(userChange->status() == MUTED) {
            chatee->mute(false);
            emit chatInfoSignal(
                    format(userChange->user(), "has been muted"));
        }
    }
    if(userChange->has_action()) {
        if(userChange->action() == JOINED) {
            chatroom_->chateeJoined(userChange->user(), serverConnection_);
            emit chatInfoSignal(
                    format(userChange->user(), "joined"));
        }
        else if(userChange->action() == LEFT) {
            chatroom_->chateeLeft(userChange->user().name());
            emit chatInfoSignal(
                    format(userChange->user(), "left"));
        }
        else if(userChange->action() == KICKED) {
            chatee->kick(false);
            emit chatInfoSignal(
                    format(userChange->user(), "has been kicked"));
        }
    }
}

void ChatClient::handleMessage(std::unique_ptr<ChatMessage> chatMessage) {
    emit chatMessageSignal(
            format(chatMessage->text()),
            format(chatMessage->from().user_name()),
            format(chatMessage->has_target() ? chatMessage->target().user_name() : ""));
}

void ChatClient::handleMessage(std::unique_ptr<ChatroomChange> chatroomChange) {
    if(chatroomChange->has_motd())
            emit chatMotdSignal(format(chatroomChange->motd()));
}

void ChatClient::handleMessage(std::unique_ptr<GenericChatResponse> response) {
    if(response->has_message()) {
        emit chatInfoSignal(
                format(response->message()));
    }
}

void ChatClient::join() {
    auto joinRequest = std::make_unique<UserJoinRequest>();

    joinRequest->set_name(clientName_);

    serverConnection_->sendMessage(std::make_unique<Message<UserJoinRequest>>(
            std::move(joinRequest), USER_JOIN_REQUEST));
}

void ChatClient::requestUserList() {
    auto userListRequest = std::make_unique<UserListRequest>();
    userListRequest->set_name(clientName_);

    serverConnection_->sendMessage(std::make_unique<Message<UserListRequest>>(
            std::move(userListRequest), USER_LIST_REQUEST));
}

QString ChatClient::format(const std::string& info) {
    return QString::fromUtf8(info.c_str());
}

QString ChatClient::format(const User& user, const char* info) {
    return format(user, QString(info));
}

QString ChatClient::format(const User& user, const std::string& info) {
    return format(user, QString::fromUtf8(info.c_str()));
}

QString ChatClient::format(const User& user, const QString& info) {
    return QString("%1 - %2").
            arg(QString::fromUtf8(user.name().c_str()), info);
}

} // SimpleChat namespace
