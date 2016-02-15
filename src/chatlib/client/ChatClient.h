#pragma once

#include <ChatMessage.pb.h>
#include "Client.h"

namespace SimpleChat {

class MessageDeserializer;

class UserJoinResponse;
class UserListResponse;

class UserChange;

class ChatMessage;
class ChatAuthorize;
class ChatCommand;

class ChatConnection;
class Chatroom;

/*!
 * Chat client implementation that handles message sending and receiving.
 *
 * ChatClient provides multiple virtual methods that will use networking
 * and events that are called by this class.
 */
class ChatClient : public Client {
public:
    ChatClient();

    virtual bool sendCommand(const std::string& command) override;
    virtual void sendMessage(const std::string& text, const std::string& target = "") override;

    /*!
     * Uses the deserializer to find the type of a received message and
     * calls one of receiveMessage overloads.
     */
    virtual void receiveUntypedMessage(const MessageDeserializer& deserializer) override;

    /*!
     * Checks if join was successful, adds a new Chatee and calls chatInfoReceived.
     */
    virtual void receiveMessage(std::unique_ptr<UserJoinResponse> joinResponse) override;
    /*!
     * Adds missing Chatees to Chatroom and calls refreshChateeList.
     */
    virtual void receiveMessage(std::unique_ptr<UserListResponse> listResponse) override;
    /*!
     * Alters the user based on the message content and calls chatInfoReceived.
     * If the UserChange.action affects the user list then refreshChateeList is called.
     */
    virtual void receiveMessage(std::unique_ptr<UserChange> userChange) override;
    /*!
     * Calls chatMessageReceived.
     */
    virtual void receiveMessage(std::unique_ptr<ChatMessage> chatMessage) override;
    /*!
     * Calls receiveMessage.
     */
    virtual void receiveMessage(std::unique_ptr<ChatroomChange> chatroomChange) override;
    /*!
     * Calls chatInfoReceived.
     */
    virtual void receiveMessage(std::unique_ptr<GenericChatResponse> response) override;

    virtual bool connectToServer() = 0;
    virtual void join() = 0;
    virtual void requestUserList() = 0;

    std::shared_ptr<Chatroom> chatroom();

    virtual std::string name() override;

protected:
    std::string clientName_;
    std::shared_ptr<Chatroom> chatroom_;

    virtual bool sendAnyMessage(std::unique_ptr<AbstractMessage> message) override = 0;
    virtual bool isConnected() override = 0;
    virtual ChatConnection* connection() override = 0;

    // event definitions
    virtual void chateeJoined(const std::string& name) override = 0;
    virtual void chateeLeft(const std::string& name) override = 0;
    virtual void chatMotdChanged(const std::string& motd) override = 0;
    virtual void chatInfoReceived(const std::string& info) override = 0;
    virtual void chatMessageReceived(const std::string& text, 
                                     const std::string& from, 
                                     const std::string& target) override = 0;

    virtual void refreshChateeList() override = 0;
    
};

} // SimpleChat namespace
