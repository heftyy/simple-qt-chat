cmake_minimum_required(VERSION 3.2)

project (chatlib)

add_subdirectory(proto)

set(CHATLIB_SOURCE_FILES
        communication/AbstractMessage.h
        communication/Message.h
        communication/MessageDeserializer.cpp
        communication/MessageDeserializer.h
        communication/MessageSerializer.cpp
        communication/MessageSerializer.h

        chat/Chatroom.cpp
        chat/Chatroom.h
        chat/Chatee.cpp
        chat/Chatee.h
        chat/ChatConnection.h

        chat/commands/CommandParser.cpp
        chat/commands/CommandParser.h
        chat/commands/MotdChatCommand.cpp
        chat/commands/MotdChatCommand.h
        chat/commands/SpecificChatCommand.h
        chat/commands/KickChatCommand.cpp
        chat/commands/KickChatCommand.h
        chat/commands/MuteChatCommand.cpp
        chat/commands/MuteChatCommand.h

        client/Client.h
        client/ChatClient.h
        client/ChatClient.cpp

        server/Server.h
        server/ChatServer.h
        server/ChatServer.cpp

        util/StringUtil.cpp
        util/StringUtil.h
        util/SmartPtrUtil.h
        )

add_library(chatlib ${CHATLIB_SOURCE_FILES})
target_link_libraries(chatlib proto)
target_include_directories(chatlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
