#pragma once

#include <iostream>
#include <memory>

#include <QObject>

#include "../server.h"

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

protected:
    virtual QHostAddress getAddress();

private slots:
    void openSession(quint16 port, QHostAddress ipAddress);
    void sendFortune();

private:
    std::shared_ptr<QTcpServer> tcpServer_;
    std::shared_ptr<QNetworkSession> networkSession_;
    QStringList fortunes;
};

}