#pragma once

#include <memory>

namespace SimpleChat {

class UserJoinResponse;
class UserListResponse;
class UserChange;
class ChatMessage;
class ChatroomChange;
class GenericChatResponse;

class AbstractMessage;
class MessageDeserializer;
class ChatConnection;

/*!
 * Interface for all chat clients.
 */
class Client {
public:
    virtual ~Client() { }

    virtual bool sendCommand(const std::string& command) = 0;
    virtual void sendMessage(const std::string& text, const std::string& target) = 0;
    virtual void updatePresence(int presence) = 0;

    virtual void receiveUntypedMessage(const MessageDeserializer& deserializer) = 0;

    virtual void receiveMessage(std::unique_ptr<UserJoinResponse> joinResponse) = 0;
    virtual void receiveMessage(std::unique_ptr<UserListResponse> listResponse) = 0;
    virtual void receiveMessage(std::unique_ptr<UserChange> userChange) = 0;
    virtual void receiveMessage(std::unique_ptr<ChatMessage> chatMessage) = 0;
    virtual void receiveMessage(std::unique_ptr<ChatroomChange> chatroomChange) = 0;
    virtual void receiveMessage(std::unique_ptr<GenericChatResponse> response) = 0;

    virtual std::string name() = 0;

protected:
    virtual bool sendAnyMessage(std::unique_ptr<AbstractMessage> message) = 0;
    virtual bool isConnected() = 0;
    virtual ChatConnection* connection() = 0;

    virtual void chateeJoined(const std::string& name) = 0;
    virtual void chateeLeft(const std::string& name) = 0;
    virtual void chatMotdChanged(const std::string& motd) = 0;
    virtual void chatInfoReceived(const std::string& info) = 0;
    virtual void chatMessageReceived(const std::string& text,
                             const std::string& from,
                             const std::string& target) = 0;

    virtual void refreshChateeList() = 0;
};

} // SimpleChat namespace
