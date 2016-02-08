cmake_minimum_required(VERSION 3.2)
set(PROJECT_NAME_STR simple_chat)

project(${PROJECT_NAME_STR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

if(UNIX)
	add_definitions(-std=c++14)
endif(UNIX)

set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/ext)

find_package(Qt5Network)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src)

enable_testing()

add_subdirectory(${EXT_PROJECTS_DIR}/gtest)
add_subdirectory(test)

if(WIN32)
	add_custom_target(Qt5CopyBinaries
		# todo: check if debug and release folder exist
		# debug version
		COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Cored.dll         ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug
		COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Networkd.dll      ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug
  
		# release version
		COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Core.dll          ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release
		COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Network.dll       ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release

		# Output Message
		COMMENT "Copying Qt binaries from '${Qt5Core_DIR}/../../bin/' to '${CMAKE_BINARY_DIR}'" VERBATIM
	)
endif(WIN32)