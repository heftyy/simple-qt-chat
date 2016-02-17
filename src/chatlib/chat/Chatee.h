#pragma once

#include <memory>
#include <string>

#include <User.pb.h>

namespace SimpleChat {

class User;
class Chatroom;
class ChatConnection;
class ChatTarget;
class AbstractMessage;

/*!
 * Chatee represents a participant of the chat.
 * All active chatees are being stored in a map in Chatroom class.
 *
 * Chatee contains a User protobuf instance.
 */
class Chatee
{
public:
    explicit Chatee(const User& user,
                    ChatConnection* connection, 
                    const std::shared_ptr<Chatroom>& chatroom);

    virtual ~Chatee();

    virtual User& user();

    /*!
     * Sends a message to this chatee using the underlying ChatConnection.
     */
    virtual bool sendMessage(std::unique_ptr<AbstractMessage> message);

    /*!
     * Sends a chat message to this chatee.
     *
     * You can also set a new target which will replace the ChatTarget
     * object included in the message. You must make sure that the target
     * exists before using this method.
     */
    virtual void sendChatMessage(const std::string& message, const std::string& from, const std::string& target = "");

    /*!
     * Send a GenericChatResponse to this chatee.
     */
    virtual void sendResponse(bool success, const std::string& message);

    /*!
     * Sets a mute flag to true.
     * If propagate is true sends a UserChange message to all other chatees.
     */
    virtual void mute(bool propagate);
    /*!
     * Sets a mute flag to false.
     * If propagate is true sends a UserChange message to all other chatees.
     */
    virtual void unmute(bool propagate);
    /*!
     * Removes chatee and closes the connection.
     * If propagate is true sends a UserChange message to all other chatees.
     */
    virtual void kick(bool propagate);

    ChatConnection* connection() const;

    bool authorized() const;
    void setAuthorized(bool authorized);

private:
    User user_;
    ChatConnection* connection_;
    std::weak_ptr<Chatroom> chatroom_;
    bool authorized_;

    /*!
     * Uses the Chatroom weak_ptr to propagate a UserChange message
     * with a given type to all other chatees.
     */
    void propagateChange(int type);

    /*!
     * Uses the Chatroom weak_ptr to get a new instance of ChatTarget.
     * Returns nullptr if this chatee doesn't exist in Chatroom.
     */
    std::unique_ptr<ChatTarget> getSelf();
    /*!
     * Uses the Chatroom weak_ptr to find the target chatee
     * and returns a new ChatTarget instance.
     * Returns nullptr if target chatee doesn't exist in Chatroom.
     */
    std::unique_ptr<ChatTarget> getTarget(const std::string& target);

    /*!
     * Builds a Message<MessageType> and sends it using the underlying ChatConnection.
     */
    template<typename MessageType>
    void prepareAndSend(std::unique_ptr<MessageType> message);
};

} // SimpleChat namespace