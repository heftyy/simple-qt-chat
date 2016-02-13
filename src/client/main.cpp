#include <QApplication>

#include "dialog/LoginDialog.h"
#include "dialog/ChatDialog.h"
#include <client/TcpChatClient.h>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
//    SimpleChat::ChatDialog dialog;
//    dialog.start();

    SimpleChat::TcpChatClient client;
    client.login("127.0.0.1", 4441, "FruFru");
    client.sendMessage("foo");
    client.sendMessage("bar");

    return app.exec();
}
