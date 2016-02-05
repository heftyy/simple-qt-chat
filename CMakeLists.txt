cmake_minimum_required(VERSION 3.2)
set(PROJECT_NAME_STR simple_chat)

project(${PROJECT_NAME_STR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/ext)

find_package(Qt5Network)

set(QT_CHAT_INCLUDES ${PROJECT_SOURCE_DIR}/include)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

enable_testing()

add_subdirectory(${EXT_PROJECTS_DIR}/gtest)
add_subdirectory(src)
add_subdirectory(test)