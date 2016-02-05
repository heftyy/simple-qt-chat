cmake_minimum_required(VERSION 3.2)

include(FindProtobuf)
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIR})

set(PROTO_FILES
        ChatMessage.proto
        )

PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER ${PROTO_FILES})
add_library(proto ${PROTO_HEADER} ${PROTO_SRC})
target_link_libraries(proto ${PROTOBUF_LIBRARY})