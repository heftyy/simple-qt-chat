cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

add_definitions(${MSVC_COMPILER_DEFS})

set(PROJECT_TEST_NAME ${PROJECT_NAME_STR}_test)
include_directories(${GTEST_INCLUDE_DIRS} ${GMOCK_INCLUDE_DIRS})

set(TEST_SRC_FILES
        mocks/MockChatConnection.h

        server/ServerTest.cpp
        server/ClientServerTest.cpp

        chatlib/resources/SerializerTest.h
        chatlib/resources/DeserializerTest.h
        chatlib/resources/AbstractMessageTest.h
        chatlib/resources/ChatroomTest.h
        chatlib/resources/CommandParserTest.h

        chatlib/SerializerTest.cpp
        chatlib/DeserializerTest.cpp
        chatlib/NetworkMessageTest.cpp
        chatlib/AbstractMessageTest.cpp
        chatlib/ChatroomTest.cpp
        chatlib/CommandParserTest.cpp
        chatlib/ChatCommandTest.cpp
        )

add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
add_dependencies(${PROJECT_TEST_NAME} googletest chatlib tcplib)

find_package(Threads)

if (NOT WIN32)
    target_link_libraries(${PROJECT_TEST_NAME}
            ${GTEST_LIBS_DIR}/libgtest.a
            ${GTEST_LIBS_DIR}/libgtest_main.a
            ${GMOCK_LIBS_DIR}/libgmock.a
            ${GMOCK_LIBS_DIR}/libgmock_main.a
            )
else ()
    target_link_libraries(${PROJECT_TEST_NAME}
            debug ${GTEST_LIBS_DIR}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
            optimized ${GTEST_LIBS_DIR}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}

            debug ${GMOCK_LIBS_DIR}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gmock${CMAKE_FIND_LIBRARY_SUFFIXES}
            optimized ${GMOCK_LIBS_DIR}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gmock${CMAKE_FIND_LIBRARY_SUFFIXES}
            )
    target_link_libraries(${PROJECT_TEST_NAME}
            debug ${GTEST_LIBS_DIR}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES}
            optimized ${GTEST_LIBS_DIR}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES}

            debug ${GMOCK_LIBS_DIR}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gmock_main${CMAKE_FIND_LIBRARY_SUFFIXES}
            optimized ${GMOCK_LIBS_DIR}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gmock_main${CMAKE_FIND_LIBRARY_SUFFIXES}
            )
endif ()

target_link_libraries(${PROJECT_TEST_NAME} ${CMAKE_THREAD_LIBS_INIT} chatlib tcplib)

add_test(test1 ${PROJECT_TEST_NAME})
