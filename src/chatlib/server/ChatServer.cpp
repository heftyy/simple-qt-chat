#include <iostream>

#include <NetworkMessage.pb.h>
#include <User.pb.h>
#include <ChatMessage.pb.h>

#include <chat/Chatroom.h>
#include <chat/Chatee.h>
#include <communication/Message.h>
#include <communication/MessageDeserializer.h>

#include "../chat/ChatConnection.h"

#include "ChatServer.h"

namespace SimpleChat {

ChatServer::ChatServer(const std::string& password) : 
        chatroom_(new Chatroom), 
        password_(password) {
    
}

void ChatServer::chateeLeft(const std::shared_ptr<Chatee>& chatee) {
    bool success;
    std::string message;

    std::tie(success, message) = chatroom_->chateeLeft(chatee->user().name());

    if (success) {
        std::cout << "SERVER:" << chatee->user().DebugString().c_str() << "left" << std::endl;

        auto userChange = std::make_unique<UserChange>();
        userChange->set_action(UserAction::LEFT);
        userChange->mutable_user()->CopyFrom(chatee->user());

        chatroom_->propagateMessage(MessageBuilder::build(std::move(userChange)));
    }
    else
        std::cerr << "removing the chatee failed" << std::endl;
}

void ChatServer::handleUntypedMessage(const MessageDeserializer& deserializer, 
                                      ChatConnection* connection) {
    if (connection == nullptr || !connection->isAlive()) {
        std::cerr << "chat connection is invalid" << std::endl;
        return;
    }

    if (!deserializer.isInitialized())
        return;

    if (deserializer.type() == USER_JOIN_REQUEST) {
        handleMessage(deserializer.getMessage<UserJoinRequest>(), connection);
        return;
    }

    if (connection->chatee() == nullptr) {
        std::cerr << "chatee not found for connection from " << 
            connection->getIdent().c_str() << std::endl;
        return;
    }

    if (deserializer.type() == USER_LIST_REQUEST) {
        handleMessage(deserializer.getMessage<UserListRequest>(), connection->chatee());
    }
    else if (deserializer.type() == USER_CHANGE) {
        handleMessage(deserializer.getMessage<UserChange>(), connection->chatee());
    }
    else if (deserializer.type() == CHAT_MESSAGE) {
        handleMessage(deserializer.getMessage<ChatMessage>(), connection->chatee());
    }
    else if (deserializer.type() == CHAT_COMMAND) {
        handleMessage(deserializer.getMessage<ChatCommand>(), connection->chatee());
    }
}

void ChatServer::handleMessage(std::unique_ptr<UserJoinRequest> joinRequest,
                               ChatConnection* connection) {
    bool success;
    std::string message;
    std::shared_ptr<Chatee> chatee;
    std::tie(success, message, chatee) = chatroom_->chateeJoined(joinRequest->name(), connection);

    auto response = std::make_unique<UserJoinResponse>();
    response->set_success(success);
    response->set_message(message);
    response->set_motd(chatroom_->motd());

    if (success) {
        response->mutable_user()->CopyFrom(chatee->user());

        auto userChange = std::make_unique<UserChange>();
        userChange->set_action(UserAction::JOINED);
        userChange->mutable_user()->CopyFrom(chatee->user());
        chatroom_->propagateMessage(MessageBuilder::build(std::move(userChange)));

        std::cout << "SERVER: " << joinRequest->DebugString().c_str() << std::endl;
    }

    connection->sendMessage(MessageBuilder::build(std::move(response)));
}

void ChatServer::handleMessage(std::unique_ptr<UserListRequest> listRequest,
                               const std::shared_ptr<Chatee>& sender) {
    auto response = std::make_unique<UserListResponse>();

    auto const& map = chatroom_->map();
    for (auto const& pair : map) {
        response->mutable_users()->Add()->CopyFrom(
                pair.second->user());
    }

    sender->sendMessage(MessageBuilder::build(std::move(response)));
}

void ChatServer::handleMessage(std::unique_ptr<UserChange> userChange,
                               const std::shared_ptr<Chatee>& sender) {
    if (userChange->has_presence())
        sender->user().set_presence(userChange->presence());

    chatroom_->propagateMessage(MessageBuilder::build(std::move(userChange)));
}

void ChatServer::handleMessage(std::unique_ptr<ChatMessage> chatMessage, const std::shared_ptr<Chatee>& sender) {
    if (chatMessage->has_target()) { //! send a private message
        auto targetChatee = chatroom_->getChatee(chatMessage->target().user_name());
        if (targetChatee == nullptr) { //! let the sender know that target doesn't exist
            auto message = "user with name " + chatMessage->target().user_name() + " doesn't exit";
            std::cout << message.c_str() << std::endl;

            sender->sendResponse(false, message);
        }
        else {
            //! tell sender - message from sender to target was sent
            sender->sendChatMessage(chatMessage->text(),
                                    sender->user().name(),
                                    chatMessage->target().user_name());

            //! tell target - message from sender was received
            targetChatee->sendChatMessage(chatMessage->text(),
                                          sender->user().name());
        }
    }
    else { //! send a public message
        if(sender->user().mute()) { //! check if the user is muted
            auto response = std::make_unique<GenericChatResponse>();
            response->set_success(false);
            response->set_message("you are muted");
            sender->sendMessage(MessageBuilder::build(std::move(response)));
        }
        else {
            chatMessage->set_allocated_from(chatroom_->getTarget(sender->user().name()).release());
            std::cout << "SERVER: " << chatMessage->DebugString().c_str() << std::endl;

            chatroom_->propagateMessage(MessageBuilder::build(std::move(chatMessage)));
        }
    }
}

void ChatServer::handleMessage(std::unique_ptr<ChatCommand> chatCommand, const std::shared_ptr<Chatee>& sender) {
    // check if the command is auth first
    if (chatCommand->type() == CommandType::AUTH) {
        if (chatCommand->arguments_size() == 1 && chatCommand->arguments(0) == password_) {
            sender->setAuthorized(true);
            sender->sendResponse(true, "auth successful");
        }
        else
            sender->sendResponse(false, "auth failure");

        return;
    }

    if (sender->authorized()) {
        if (chatCommand->type() == CommandType::MUTE) {
            auto targetChatee = chatroom_->getChatee(chatCommand->arguments(0));
            if (targetChatee == nullptr)
                sender->sendResponse(false, "user " + chatCommand->arguments(0) + " doesn't exist");
            targetChatee->mute(true);
        }
        else if (chatCommand->type() == CommandType::UNMUTE) {
            auto targetChatee = chatroom_->getChatee(chatCommand->arguments(0));
            if (targetChatee == nullptr)
                sender->sendResponse(false, "user " + chatCommand->arguments(0) + " doesn't exist");
            targetChatee->unmute(true);
        }
        else if (chatCommand->type() == CommandType::KICK) {
            auto targetChatee = chatroom_->getChatee(chatCommand->arguments(0));
            if (targetChatee == nullptr)
                sender->sendResponse(false, "user " + chatCommand->arguments(0) + " doesn't exist");
            targetChatee->kick(true);
        }
        else if (chatCommand->type() == CommandType::MOTD) {
            chatroom_->setMotd(chatCommand->arguments(0));
        }
    }
    else
        sender->sendResponse(false, "you are not authorized to use this command");
}

} // SimpleChat namespace
