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

TcpChatClient::TcpChatClient(QObject* parent) :
        QObject(parent),
        serverConnection_(nullptr),
        serverAddress_(QHostAddress::LocalHost),
        serverPort_(0) {

}

TcpChatClient::~TcpChatClient() {
    serverConnection_->deleteLater();
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

ChatConnection* TcpChatClient::connection() {
    return serverConnection_;
}

void TcpChatClient::chateeJoined(const std::string& name) {
    emit chateeJoinedSignal(format(name));

    qDebug() << name.c_str() << "joined";
}

void TcpChatClient::chateeLeft(const std::string& name) {
    emit chateeLeftSignal(format(name));

    qDebug() << name.c_str() << "left";
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
    emit chatInfoSignal(format("disconnected, connection lost"));
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

    auto socket = new QTcpSocket(this);
    auto connection = new TcpChatConnection(socket, this);
    connection->init();

    connection->connect(connection,
                        SIGNAL(dataReceivedSignal(QString)),
                        this,
                        SLOT(handleUntypedMessage(QString)));

    connection->connect(connection,
                        SIGNAL(disconnectSignal()),
                        this,
                        SLOT(connectionLost()));

    connection->connect(connection->socket(),
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

QString TcpChatClient::format(const std::string& info) const {
    return QString::fromUtf8(info.c_str());
}

QString TcpChatClient::format(const User& user, const char* info) const {
    return format(user, QString(info));
}

QString TcpChatClient::format(const User& user, const std::string& info) const {
    return format(user, QString::fromUtf8(info.c_str()));
}

QString TcpChatClient::format(const User& user, const QString& info) const {
    return QString("%1 - %2").
        arg(QString::fromUtf8(user.name().c_str()), info);
}

} // SimpleChat namespace
