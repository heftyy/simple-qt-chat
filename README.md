Simple Chat Docs
======

Server
-----

Simple Chat server is a single-threaded command line application.

Available command line options:
* -h, --help                         Displays this help.
* -v, --version                      Displays version information.
* -i, --ip-address <ip address>      IP address for server to listen on.
* -p, --port <port>                  Port for server to listen on.
* -s, --secret, --password <secret>  Secret used by users to authorize.

Client
-----
Simple Chat client is a single-threaded GUI application.

After starting the client you have to provide
* ip address of the server
* port of the server
* nickname

Client will attempt to join the server and display a chat dialog on success.

Command available in the chat dialog:
* ```/auth <secret> authorizes``` you on the server
* ```/mute <nickname>``` mutes a chat participant
* ```/unmute <nickname>``` unmutes a chat participant
* ```/kick <nicnname>``` kicks a chat participant
* ```/motd <motd>``` sets a new motd for chat
* ```/w <nickname> <message>``` send a private message

Users can change their status using the dropdown below the list of chat participants.
* ```green``` online
* ```yellow``` away
* ```red``` busy


toolchains tested:
* gcc >= 4.9
* msvc 14

dependencies:
* cmake >= 3.2
* Qt >= 5.5
* google protocol buffers >= 2.6.1
* gtest & gmock
* doxygen (only if you want to use make doc)