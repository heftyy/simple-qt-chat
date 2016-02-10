cmake_minimum_required(VERSION 3.2)

set(SHARED_DIRECTORY shared)
set(CLIENT_DIRECTORY client)
set(SERVER_DIRECTORY server)

include_directories(shared)

add_subdirectory(shared)
add_subdirectory(client)
add_subdirectory(server)
