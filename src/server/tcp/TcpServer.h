#pragma once

#include <iostream>
#include <memory>

#include <QObject>

#include "Server.h"

QT_BEGIN_NAMESPACE
class QTcpServer;
class QNetworkSession;
QT_END_NAMESPACE

namespace SimpleChat {

class TcpServer : public Server {
    Q_OBJECT
public:
    TcpServer();

    virtual void listen(quint16 port, QHostAddress ipAddress = QHostAddress::LocalHost) override;
	virtual void sendMessage(std::unique_ptr<AbstractMessage> message) override;

protected:
    virtual QHostAddress getAddress() override;

private slots:
    void sendFortune() const;

private:
    std::shared_ptr<QTcpServer> tcpServer_;
    std::shared_ptr<QNetworkSession> networkSession_;
    QStringList fortunes;

    void openSession(quint16 port, QHostAddress ipAddress);
};

}