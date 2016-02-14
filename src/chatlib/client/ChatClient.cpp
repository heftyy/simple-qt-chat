#include <iostream>

#include <User.pb.h>
#include <ChatMessage.pb.h>
#include <NetworkMessage.pb.h>
#include <communication/Message.h>
#include <communication/MessageDeserializer.h>
#include <chat/ChatConnection.h>
#include <chat/Chatroom.h>
#include <chat/Chatee.h>
#include <chat/commands/CommandParser.h>

#include "ChatClient.h"

namespace SimpleChat {

ChatClient::ChatClient() : 
    chatroom_(new Chatroom) {
    
}

bool ChatClient::sendCommand(const std::string& command) {
    CommandParser commandParser(command);

    auto message = commandParser.chatCommand(chatroom_->getTarget(clientName_));
    if(message) {
        auto result = sendAnyMessage(
            MessageBuilder::build(std::move(message)));

        if (!result) {
            std::cerr << "sending a command failed" << std::endl;
            return false;
        }
        return true;
    }

    return false;
}

void ChatClient::sendMessage(const std::string& text, const std::string& target) {
    auto chatMessage = std::make_unique<ChatMessage>();
    chatMessage->set_text(text);
    if(!target.empty()) {
        chatMessage->set_allocated_target(chatroom_->getTarget(target).release());
    }

    auto result = sendAnyMessage(
            MessageBuilder::build(std::move(chatMessage)));

    if (!result)
        std::cerr << "sending a message failed" << std::endl;
}

void ChatClient::handleUntypedMessage(const MessageDeserializer& deserializer) {

    if (!isConnected()) {
        std::cerr << "chat connection is invalid" << std::endl;
        return;
    }
    
    if (!deserializer.isInitialized())
        return;

    if (deserializer.type() == USER_JOIN_RESPONSE) {
        handleMessage(deserializer.getMessage<UserJoinResponse>());
    }
    else if (deserializer.type() == USER_LIST_RESPONSE) {
        handleMessage(deserializer.getMessage<UserListResponse>());
    }
    else if (deserializer.type() == USER_CHANGE) {
        handleMessage(deserializer.getMessage<UserChange>());
    }
    else if (deserializer.type() == CHAT_MESSAGE) {
        handleMessage(deserializer.getMessage<ChatMessage>());
    }
    else if(deserializer.type() == CHATROOM_CHANGE) {
        handleMessage(deserializer.getMessage<ChatroomChange>());
    }
    else if(deserializer.type() == GENERIC_CHAT_RESPONSE) {
        handleMessage(deserializer.getMessage<GenericChatResponse>());
    }
}

void ChatClient::handleMessage(std::unique_ptr<UserJoinResponse> joinResponse) {
    std::cout << "joined successfully: " <<
        joinResponse->DebugString().c_str() << std::endl;;

    if(joinResponse->success()) {
        requestUserList();
        chatroom_->chateeJoined(joinResponse->user(), connection());

        chatMotdChanged(joinResponse->motd());
    }
    else {
        chatInfoReceived(joinResponse->message());
    }
}

void ChatClient::handleMessage(std::unique_ptr<UserListResponse> listResponse) {
    for(auto const& user : listResponse->users()) {
        chatroom_->chateeJoined(user, connection());
    }

    refreshChateeList();
}

void ChatClient::handleMessage(std::unique_ptr<UserChange> userChange) {
    std::cout << "user change " <<
        userChange->DebugString().c_str() << std::endl;

    // check for join first because chatee doesn't exist yet
    if (userChange->has_action()) {
        if (userChange->action() == JOINED) {
            chatroom_->chateeJoined(userChange->user(), connection());
            chatInfoReceived(userChange->user().name() + " joined");
            refreshChateeList();

            return;
        }
    }

    auto chatee = chatroom_->getChatee(userChange->user().name());
    if(chatee == nullptr)
        return;

    if(userChange->has_presence()) {
        chatee->user().set_presence(userChange->presence());
        chatInfoReceived(
            userChange->user().name() + " is now " + UserPresence_Name(userChange->presence()));
    }
    if(userChange->has_status()) {
        chatee->user().set_status(userChange->status());
        if(userChange->status() == MUTED) {
            chatee->mute(false);
            chatInfoReceived(userChange->user().name() + " has been muted");
        }
    }
    if(userChange->has_action()) {
        if(userChange->action() == LEFT) {
            chatroom_->chateeLeft(userChange->user().name());
            chatInfoReceived(userChange->user().name() + " left");
            refreshChateeList();
        }
        else if(userChange->action() == KICKED) {
            chatee->kick(false);
            chatInfoReceived(userChange->user().name() + " has been kicked");
            refreshChateeList();
        }
    }
}

void ChatClient::handleMessage(std::unique_ptr<ChatMessage> chatMessage) {
    chatMessageReceived(chatMessage->text(), 
                        chatMessage->from().user_name(), 
                        chatMessage->has_target() ? chatMessage->target().user_name() : "");
}

void ChatClient::handleMessage(std::unique_ptr<ChatroomChange> chatroomChange) {
    if (chatroomChange->has_motd())
        chatMotdChanged(chatroomChange->motd());
}

void ChatClient::handleMessage(std::unique_ptr<GenericChatResponse> response) {
    if(response->has_message()) {
        chatInfoReceived(response->message());
    }
}

void ChatClient::join() {
    auto joinRequest = std::make_unique<UserJoinRequest>();

    joinRequest->set_name(clientName_);

    sendAnyMessage(MessageBuilder::build(std::move(joinRequest)));
}

void ChatClient::requestUserList() {
    auto userListRequest = std::make_unique<UserListRequest>();
    userListRequest->set_name(clientName_);

    sendAnyMessage(MessageBuilder::build(std::move(userListRequest)));
}

std::shared_ptr<Chatroom> ChatClient::chatroom() {
    return chatroom_;
}

} // SimpleChat namespace
