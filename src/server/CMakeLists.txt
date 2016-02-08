cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set(SERVER_SOURCE_FILES
        main.cpp
        Server.h
        tcp/TcpServer.h
        tcp/TcpServer.cpp
        tcp/TcpChatConnection.h
        tcp/TcpChatConnection.cpp
        )

add_executable(chat_server ${SERVER_SOURCE_FILES})
target_link_libraries(chat_server Qt5::Network shared)