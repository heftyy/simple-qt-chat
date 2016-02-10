cmake_minimum_required(VERSION 3.2)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set(SERVER_SOURCE_FILES
        main.cpp
        Server.h
        tcp/TcpServer.cpp
        tcp/TcpServer.h
        tcp/TcpChatConnection.cpp
        tcp/TcpChatConnection.h
        )

add_executable(chat_server ${SERVER_SOURCE_FILES})
target_link_libraries(chat_server Qt5::Network shared)