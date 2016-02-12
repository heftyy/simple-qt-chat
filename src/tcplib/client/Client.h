#pragma once

#include <memory>
#include <QtNetwork>

namespace SimpleChat {

class Client : public QObject {
public:
    virtual ~Client() { }
};

} // SimpleChat namespace
