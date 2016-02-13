#include <QtWidgets>

#include <client/TcpChatClient.h>
#include <chat/Chatroom.h>
#include <chat/Chatee.h>

#include "ChatDialog.h"
#include "LoginDialog.h"

namespace SimpleChat {

ChatDialog::ChatDialog(QWidget* parent) :
    QDialog(parent),
    loginDialog(new LoginDialog),
    chatClient(new TcpChatClient) {

}

ChatDialog::~ChatDialog() {
    loginDialog->deleteLater();
    chatClient->deleteLater();
    qDebug() << "deleted chat dialog";
}

void ChatDialog::start() const {
    loginDialog->setupDialog();
    loginDialog->show();

    connect(loginDialog, SIGNAL(loginSignal(QString, quint16, QString)),
            this, SLOT(loginToChat(QString, quint16, QString)));
}

void ChatDialog::appendMessage(const QString& message, const QString& from) const {
    if (from.isEmpty() || message.isEmpty())
        return;

    auto cursor(textEdit->textCursor());
    cursor.movePosition(QTextCursor::End);
    auto table = cursor.insertTable(1, 2, tableFormat);
    table->cellAt(0, 0).firstCursorPosition().insertText('<' + from + "> ");
    table->cellAt(0, 1).firstCursorPosition().insertText(message);
    auto bar = textEdit->verticalScrollBar();
    bar->setValue(bar->maximum());
}

void ChatDialog::appendInfo(const QString& info) const {
    if (info.isEmpty())
        return;

    auto color = textEdit->textColor();
    textEdit->setTextColor(Qt::gray);
    textEdit->append("* " + info);
    textEdit->setTextColor(color);
}

void ChatDialog::appendMotd(const QString& motd) const {
    if (motd.isEmpty())
        return;

    auto color = textEdit->textColor();
    textEdit->setTextColor(Qt::gray);
    textEdit->append(tr("* motd: %1").arg(motd));
    textEdit->setTextColor(color);
}

void ChatDialog::refreshList() {
    // remove chatees from the widget list
    for (auto i = 0; i < listWidget->count(); ++i) {
        auto item = listWidget->item(i);
        auto name = item->text().toStdString();
        if(!chatClient->chatroom()->chateeExists(name)) {
            delete item;
        }
    }

    // add chatees to the widget list
    for (auto const& entry : chatClient->chatroom()->map()) {        
        auto name = QString::fromStdString(entry.second->user().name());
        auto items = listWidget->findItems(name,
                                           Qt::MatchExactly);
        if (items.isEmpty())
            listWidget->addItem(name);
    }    
}

void ChatDialog::returnPressed() const {
    auto text = lineEdit->text();
    if (text.isEmpty())
        return;
    
    if (text.startsWith(QChar('/'))) {
        qDebug() << "command" << text;
        auto color = textEdit->textColor();
        textEdit->setTextColor(Qt::red);

        auto success = chatClient->sendCommand(text.toStdString());
        if (success)
            textEdit->append(tr("* sending command: %1").arg(text));
        else
            textEdit->append(tr("* commmand unrecognized, try /help"));

        textEdit->setTextColor(color);
    }
    else {
        qDebug() << "message" << text;
        chatClient->sendMessage(text.toStdString());
    }

    lineEdit->clear();
}

void ChatDialog::showInformation() {
    if (listWidget->count() == 1) {
        QMessageBox::information(this, tr("Chat"),
                                 tr("Launch several instances of this "
                                 "program on your local network and "
                                 "start chatting!"));
    }
}

void ChatDialog::loginToChat(const QString& address, quint16 port, const QString& name) {
    chatClient->login(address, port, name);
    loginDialog->hide();
    this->showChat();
}

void ChatDialog::showChat() {
    setupUi(this);
    this->show();

    lineEdit->setFocusPolicy(Qt::StrongFocus);
    textEdit->setFocusPolicy(Qt::NoFocus);
    textEdit->setReadOnly(true);
    listWidget->setFocusPolicy(Qt::NoFocus);
    
    connect(chatClient, SIGNAL(chatMessageSignal(QString, QString, QString)),
            this, SLOT(appendMessage(QString, QString)));
    connect(chatClient, SIGNAL(chatInfoSignal(QString)),
            this, SLOT(appendInfo(QString)));
    connect(chatClient, SIGNAL(chatMotdSignal(QString)),
            this, SLOT(appendMotd(QString)));
    connect(chatClient, SIGNAL(chatRefreshListSignal()),
            this, SLOT(refreshList()));
    connect(lineEdit, SIGNAL(returnPressed()),
            this, SLOT(returnPressed()));

    tableFormat.setBorder(0);
}

} // SimpleChat namespace