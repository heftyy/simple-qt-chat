cmake_minimum_required(VERSION 3.2)

project (shared)

add_subdirectory(proto)

set(SHARED_SOURCE_FILES
        communication/AbstractMessage.h
        communication/Message.h
        communication/Message.cpp
        communication/MessageDeserializer.h
        communication/MessageDeserializer.cpp
        communication/MessageSerializer.h
        communication/MessageSerializer.cpp

        chat/Chatroom.h
        chat/Chatroom.cpp
        chat/Chatee.h
        chat/Chatee.cpp
        chat/ChatConnection.h

        chat/commands/CommandParser.h
        chat/commands/CommandParser.cpp

        util/StringUtil.h
        util/StringUtil.cpp
        util/SmartPtrUtil.h
        chat/commands/MotdChatCommand.cpp chat/commands/MotdChatCommand.h chat/commands/SpecificChatCommand.h chat/commands/KickChatCommand.cpp chat/commands/KickChatCommand.h chat/commands/MuteChatCommand.cpp chat/commands/MuteChatCommand.h)

add_library(shared ${SHARED_SOURCE_FILES})
target_link_libraries(shared proto)
