#pragma once

#include <memory>

#include <QObject>
#include <QtNetwork/qhostaddress.h>

namespace SimpleChat {

class Server : public QObject {
public:
    Server() { };

    virtual void listen(quint16 port, QHostAddress ipAddress = QHostAddress::LocalHost) = 0;
    virtual ~Server() { }

protected:
    virtual QHostAddress getAddress() = 0;
};

}