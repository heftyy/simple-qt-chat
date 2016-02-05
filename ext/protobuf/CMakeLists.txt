cmake_minimum_required(VERSION 3.2)
project(protobuf_builder C CXX)
include(ExternalProject)

ExternalProject_Add(protobuf
        GIT_REPOSITORY https://github.com/google/protobuf.git
        CMAKE_ARGS -DCMAKE_BUILD_TYPE=Debug
        PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
        # Disable install step
        INSTALL_COMMAND ""
)

# Specify include dir
#ExternalProject_Get_Property(googletest source_dir)
#set(PROTOBUF_INCLUDE_DIRS ${source_dir}/googletest/include PARENT_SCOPE)

# Specify MainTest's link libraries
#ExternalProject_Get_Property(googletest binary_dir)
#set(PROTOBUF_LIBS_DIR ${binary_dir}/googlemock/gtest PARENT_SCOPE)