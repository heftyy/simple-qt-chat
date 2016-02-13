cmake_minimum_required(VERSION 3.2)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

project (tcplib)

set(TCPLIB_SOURCE_FILES
        chat/TcpChatConnection.cpp
        chat/TcpChatConnection.h

        server/TcpChatServer.cpp
        server/TcpChatServer.h

        client/TcpChatClient.cpp
        client/TcpChatClient.h
        )

add_library(tcplib ${TCPLIB_SOURCE_FILES})
target_link_libraries(tcplib Qt5::Network Qt5::Widgets chatlib)
target_include_directories(tcplib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
