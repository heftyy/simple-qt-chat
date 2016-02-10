cmake_minimum_required(VERSION 3.2)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

QT5_WRAP_UI(UI_HEADERS
        dialog/chatdialog.ui
        )

set(CLIENT_SOURCE_FILES
        ${UI_HEADERS}
        main.cpp
        Client.h
        tcp/TcpClient.cpp
        tcp/TcpClient.h
        tcp/TcpChatConnection.cpp
        tcp/TcpChatConnection.h
        )

add_executable(chat_client ${CLIENT_SOURCE_FILES})
target_link_libraries(chat_client Qt5::Network Qt5::Widgets shared)