cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${QT_CHAT_INCLUDES})

set(SERVER_SOURCE_FILES
        main.cpp
        server.h
        tcp/tcp_server.h
        tcp/tcp_server.cpp
        )

add_executable(chat_server ${SERVER_SOURCE_FILES})

target_link_libraries(chat_server Qt5::Network)