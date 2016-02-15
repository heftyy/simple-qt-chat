#pragma once

#include <memory>
#include <string>

namespace SimpleChat {

class AbstractMessage;

/*!
 * MessageSerializer holds the AbstractMessage interface.
 * You can use serialize() method multiple times.
 */
class MessageSerializer {
public:
    explicit MessageSerializer(std::unique_ptr<AbstractMessage> abstractMessage);

    /*!
     * Returns:
     *   - bool success
     *   - std::string serialized message
     */
    std::tuple<bool, std::string> serialize() const;

private:
    std::unique_ptr<AbstractMessage> abstractMessage_;
};

} // SimpleChat namespace
