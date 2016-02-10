cmake_minimum_required(VERSION 3.2)

set(SERVER_SOURCE_FILES
        main.cpp
        )

add_executable(chat_server ${SERVER_SOURCE_FILES})
target_link_libraries(chat_server chatlib tcplib)
