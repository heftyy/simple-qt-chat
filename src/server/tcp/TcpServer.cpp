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

TcpServer::TcpServer()
        : tcpServer_(nullptr),
          chatroom_(std::make_shared<Chatroom>()) {

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


void TcpServer::connectionEstablished() const {
	auto clientConnection = tcpServer_->nextPendingConnection();
	std::shared_ptr<QTcpSocket> tcpSocket(clientConnection);

    auto connection = std::make_shared<TcpChatConnection>(
            std::shared_ptr<QTcpSocket>(clientConnection)
    );

	clientConnection->
		connect(clientConnection, SIGNAL(readyRead()), 
				clientConnection, SLOT(dataReceived(connection)));

	clientConnection->
		connect(clientConnection, SIGNAL(disconnected()),
				clientConnection, SLOT(deleteLater()));	
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

void TcpServer::listen(quint16 port, QHostAddress ipAddress) {
	openSession(port, ipAddress);

	connect(tcpServer_.get(), SIGNAL(newConnection()),
			this, SLOT(connectionEstablished()));
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

void TcpServer::handleUntypedMessage(
        const MessageDeserializer& deserializer,
        const std::shared_ptr<ChatConnection>& connection) {

    if(!deserializer.isInitialized())
        return;

    if(deserializer.type() == USER_JOIN_REQUEST) {
        handleMessage(deserializer.getMessage<UserJoinRequest>(), connection);
    }
    else if(deserializer.type() == USER_JOIN_RESPONSE) {
        handleMessage(deserializer.getMessage<UserJoinResponse>());
    }
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

    if(success)
        response->set_allocated_user(&chatee->user());

    auto responseMessage = std::make_unique<Message<UserJoinResponse>>(std::move(response), USER_JOIN_RESPONSE);

    chatroom_->propagateMessage(std::move(responseMessage));

    std::cout << "user " << joinRequest->name() << " joined" << std::endl;
}

void TcpServer::handleMessage(std::unique_ptr<UserJoinResponse> joinResponse) {

}

}