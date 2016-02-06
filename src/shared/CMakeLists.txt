cmake_minimum_required(VERSION 3.2)

project (shared)

add_subdirectory(proto)

set(SHARED_SOURCE_FILES
        communication/AbstractMessage.h
        communication/Message.h
        communication/Message.cpp

		chat/Chatroom.h
		chat/Chatroom.cpp
		chat/Chatee.h
		chat/Chatee.cpp
        )

add_library(shared ${SHARED_SOURCE_FILES})
target_include_directories(shared PUBLIC ${SHARED_SOURCE_FILES})
target_link_libraries(shared proto)
