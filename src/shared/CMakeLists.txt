cmake_minimum_required(VERSION 3.2)

include_directories(${QT_CHAT_INCLUDES})

set(SHARED_SOURCE_FILES
        communication/AbstractMessage.h
        communication/AbstractMessage.cpp
        communication/Message.h
        communication/Message.cpp
        )

add_subdirectory(proto)

include_directories(${PROTO_HEADER})

add_library(shared ${SHARED_SOURCE_FILES})
target_link_libraries(shared proto)