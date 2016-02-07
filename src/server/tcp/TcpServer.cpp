#include "TcpServer.h"

#include <QtNetwork>

namespace SimpleChat {

TcpServer::TcpServer() : tcpServer_(nullptr), networkSession_(nullptr) {
	chatroom_ = std::make_shared<Chatroom>();
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


void TcpServer::connectionEstabilished() const {
	auto clientConnection = tcpServer_->nextPendingConnection();
	std::shared_ptr<QTcpSocket> socket(clientConnection);

	clientConnection->
		connect(clientConnection, SIGNAL(readyRead()), 
				clientConnection, SLOT(dataReceived(socket)));

	clientConnection->
		connect(clientConnection, SIGNAL(disconnected()),
				clientConnection, SLOT(deleteLater()));	
}

void TcpServer::dataReceived(const std::shared_ptr<QTcpSocket>& tcpSocket) {
	QDataStream inStream(tcpSocket.get());
	inStream.setVersion(QDataStream::Qt_5_5);

	quint16 blockSize = 0;

	if (blockSize == 0) {
		if (tcpSocket->bytesAvailable() < static_cast<int>(sizeof(quint16)))
			return;

		inStream >> blockSize;
	}

	if (tcpSocket->bytesAvailable() < blockSize)
		return;

	QString serializedMessage;
	inStream >> serializedMessage;

}

void TcpServer::listen(quint16 port, QHostAddress ipAddress) {
	openSession(port, ipAddress);

	connect(tcpServer_.get(), SIGNAL(newConnection()),
			this, SLOT(connectionEstabilished()));
}

void TcpServer::sendMessage(std::unique_ptr<AbstractMessage> message) {

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

}