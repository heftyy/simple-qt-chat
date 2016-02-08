#pragma once

#include <memory>
#include <string>

namespace SimpleChat {

class AbstractMessage;

class MessageSerializer {
public:
    explicit MessageSerializer(std::unique_ptr<AbstractMessage> abstractMessage,
                               const std::string& dest,
                               const std::string& src);

    std::tuple<bool, std::string> serialize() const;

private:
    std::unique_ptr<AbstractMessage> abstractMessage_;
    std::string dest_;
    std::string src_;
};

} //SimpleChat namespace



