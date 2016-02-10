cmake_minimum_required(VERSION 3.2)

set(CHATLIB_DIRECTORY chatlib)
set(TCPLIB_DIRECTORY tcplib)
set(CLIENT_DIRECTORY client)
set(SERVER_DIRECTORY server)

add_subdirectory(chatlib)
add_subdirectory(tcplib)
add_subdirectory(client)
add_subdirectory(server)
