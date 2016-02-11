#pragma once

#include <memory>
#include <string>

namespace SimpleChat {

class AbstractMessage;

class MessageSerializer {
public:
    explicit MessageSerializer(std::unique_ptr<AbstractMessage> abstractMessage);

    std::tuple<bool, std::string> serialize() const;

private:
    std::unique_ptr<AbstractMessage> abstractMessage_;
};

} // SimpleChat namespace



