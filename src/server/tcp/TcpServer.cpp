#include "TcpServer.h"

#include <QtNetwork>

namespace SimpleChat {

TcpServer::TcpServer()
        : tcpServer_(nullptr),
          networkSession_(nullptr) {
}

void TcpServer::openSession(quint16 port, QHostAddress ipAddress) {

    if(ipAddress == QHostAddress::LocalHost)
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


void TcpServer::sendFortune() {

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_5);

    out << (quint16) 0;
    out << fortunes.at(qrand() % fortunes.size());
    out.device()->seek(0);
    out << (quint16) (block.size() - sizeof(quint16));


    QTcpSocket* clientConnection = tcpServer_->nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));

    clientConnection->write(block);
    clientConnection->disconnectFromHost();

}

void TcpServer::listen(quint16 port, QHostAddress ipAddress) {

    openSession(port, ipAddress);

    connect(tcpServer_.get(), SIGNAL(newConnection()),
            this, SLOT(sendFortune()));
}

QHostAddress TcpServer::getAddress() {

    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (auto ipAddress : ipAddressesList) {
        if (ipAddress != QHostAddress::LocalHost && ipAddress.toIPv4Address()) {
            return ipAddress;
        }
    }

    return QHostAddress(QHostAddress::LocalHost);
}

}