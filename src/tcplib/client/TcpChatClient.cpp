#include <iostream>

#include <QtNetwork>

#include <User.pb.h>
#include <ChatMessage.pb.h>
#include <NetworkMessage.pb.h>
#include <communication/Message.h>
#include <communication/MessageDeserializer.h>
#include <chat/TcpChatConnection.h>

#include "TcpChatClient.h"

namespace SimpleChat {

TcpChatClient::TcpChatClient() :
    serverConnection_(nullptr),
    serverAddress_(QHostAddress::LocalHost),
    serverPort_(0) {

}

bool TcpChatClient::login(const QString& address, quint16 port, const QString& name) {
    serverAddress_ = QHostAddress(address);
    serverPort_ = port;
    clientName_ = name.toStdString();

    return connectToServer();
}

void TcpChatClient::logout() {
    serverConnection_->disconnectFromHost();
}

bool TcpChatClient::sendAnyMessage(std::unique_ptr<AbstractMessage> message) {
    if(serverConnection_ != nullptr) 
        return serverConnection_->sendMessage(std::move(message));
    return false;
}

bool TcpChatClient::isConnected() {
    return serverConnection_ != nullptr && serverConnection_->isAlive();
}

std::shared_ptr<ChatConnection> TcpChatClient::connection() {
    return serverConnection_;
}

void TcpChatClient::chatMotdChanged(const std::string& motd) {
    emit chatMotdSignal(format(motd));

    qDebug() << "motd" << motd.c_str();
}

void TcpChatClient::chatInfoReceived(const std::string& info) {
    emit chatInfoSignal(format(info));

    qDebug() << "info" << info.c_str();
}

void TcpChatClient::chatMessageReceived(const std::string& text, const std::string& from, const std::string& target) {
    emit chatMessageSignal(
        format(text),
        format(from),
        format(target));

    qDebug() << "message" << from.c_str() << text.c_str() << target.c_str();
}

void TcpChatClient::refreshChateeList() {
    emit chatRefreshListSignal();

    qDebug() << "list refresh";
}

void TcpChatClient::handleUntypedMessage(const QString& serializedData) {

    auto deserializer = MessageDeserializer(serializedData.toStdString());
    ChatClient::handleUntypedMessage(deserializer);
}

void TcpChatClient::connectionLost() {
    qDebug() << "disconnected\n";
}

void TcpChatClient::displayError(QAbstractSocket::SocketError socketError) const {
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

bool TcpChatClient::connectToServer() {
    if (clientName_.empty() || serverAddress_.isNull() || serverPort_ <= 0) {
        return false;
    }

    auto socket = std::make_shared<QTcpSocket>();
    auto connection = std::make_shared<TcpChatConnection>(socket);
    connection->init();

    connection->connect(connection.get(),
                        SIGNAL(dataReceivedSignal(QString)),
                        this,
                        SLOT(handleUntypedMessage(QString)));

    connection->connect(connection.get(),
                        SIGNAL(disconnectSignal()),
                        this,
                        SLOT(connectionLost()));

    connection->connect(connection->socket().get(),
                        SIGNAL(error(QAbstractSocket::SocketError)),
                        this,
                        SLOT(displayError(QAbstractSocket::SocketError)));

    socket->connectToHost(serverAddress_, serverPort_);
    if (socket->waitForConnected()) {
        serverConnection_ = connection;
        join();
        return true;
    }
    return false;
}

void TcpChatClient::join() {
    auto joinRequest = std::make_unique<UserJoinRequest>();

    joinRequest->set_name(clientName_);

    serverConnection_->sendMessage(std::make_unique<Message<UserJoinRequest>>(
        std::move(joinRequest), USER_JOIN_REQUEST));
}

void TcpChatClient::requestUserList() {
    auto userListRequest = std::make_unique<UserListRequest>();
    userListRequest->set_name(clientName_);

    serverConnection_->sendMessage(std::make_unique<Message<UserListRequest>>(
        std::move(userListRequest), USER_LIST_REQUEST));
}

QString TcpChatClient::format(const std::string& info) {
    return QString::fromUtf8(info.c_str());
}

QString TcpChatClient::format(const User& user, const char* info) {
    return format(user, QString(info));
}

QString TcpChatClient::format(const User& user, const std::string& info) {
    return format(user, QString::fromUtf8(info.c_str()));
}

QString TcpChatClient::format(const User& user, const QString& info) {
    return QString("%1 - %2").
        arg(QString::fromUtf8(user.name().c_str()), info);
}

} // SimpleChat namespace
