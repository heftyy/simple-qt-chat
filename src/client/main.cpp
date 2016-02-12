#include <QApplication>

#include "dialog/LoginDialog.h"
#include "dialog/ChatDialog.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    SimpleChat::ChatDialog dialog;
    dialog.start();

    return app.exec();
}
