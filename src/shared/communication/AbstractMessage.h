#pragma once

#include <string>
#include <memory>

namespace SimpleChat {

class AbstractMessage {
public:
    virtual std::string serialize() = 0;
};

}

