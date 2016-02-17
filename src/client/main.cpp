#include <QApplication>

#include <SimpleChatConfig.h>
#include <client/TcpChatClient.h>
#include "dialog/LoginDialog.h"
#include "dialog/ChatDialog.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName("simple chat server");
    QApplication::setApplicationVersion(QString("%1.%2.%3")
                                                    .arg(SimpleChat_VERSION_MAJOR)
                                                    .arg(SimpleChat_VERSION_MINOR)
                                                    .arg(SimpleChat_VERSION_PATCH));
    SimpleChat::ChatDialog dialog(app.activeWindow());
    dialog.start();

    return app.exec();
}
