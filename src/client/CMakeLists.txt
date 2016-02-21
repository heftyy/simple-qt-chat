cmake_minimum_required(VERSION 3.2)

set(CMAKE_AUTOMOC ON)

QT5_WRAP_UI(UI_HEADERS
        dialog/chatdialog.ui
        )

set(CLIENT_SOURCE_FILES
        ${UI_HEADERS}
        main.cpp
        dialog/ChatDialog.cpp
        dialog/ChatDialog.h
        dialog/LoginDialog.cpp
        dialog/LoginDialog.h)

add_executable(chat_client WIN32 ${CLIENT_SOURCE_FILES})
target_link_libraries(chat_client ${QT_QTMAIN_LIBRARY} Qt5::Network Qt5::Widgets chatlib tcplib)

IF(WIN32) # Check if we are on Windows
  if(MSVC) # Check if we are using the Visual Studio compiler
    set_target_properties(chat_client PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
  else()
    message(SEND_ERROR "You are using an unsupported Windows compiler! (Not MSVC)")
  endif()
elseif(UNIX)
  # Nothing special required
else()
  message(SEND_ERROR "You are on an unsupported platform! (Not Win32 or Unix)")
ENDIF()
