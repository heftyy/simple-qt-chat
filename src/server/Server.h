#pragma once

#include <memory>

#include <QObject>
#include <QtNetwork/qhostaddress.h>

#include "communication/AbstractMessage.h"

namespace SimpleChat {

class Server : public QObject {
public:
    Server() { };

    virtual void listen(quint16 port, QHostAddress ipAddress = QHostAddress::LocalHost) = 0;
	virtual void sendMessage(std::unique_ptr<AbstractMessage> message) = 0;
    virtual ~Server() { }

protected:
    virtual QHostAddress getAddress() = 0;
};

}