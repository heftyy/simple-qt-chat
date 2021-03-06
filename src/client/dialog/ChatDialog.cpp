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

void ChatDialog::appendMessage(const QString& message, const QString& from, const QString& target) const {
    if (from.isEmpty() || message.isEmpty())
        return;

    if(target.isEmpty()) { // global message
        auto cursor(textEdit->textCursor());
        cursor.movePosition(QTextCursor::End);
        auto table = cursor.insertTable(1, 2, tableFormat);
        table->cellAt(0, 0).firstCursorPosition().insertText('<' + from + "> ");
        table->cellAt(0, 1).firstCursorPosition().insertText(message);
        auto bar = textEdit->verticalScrollBar();
        bar->setValue(bar->maximum());
    }
    else { // whisper message
        auto color = textEdit->textColor();
        textEdit->setTextColor(QColor(120, 0, 150, 155));
        textEdit->append(tr("* %1: %2").arg(from, message));
        textEdit->setTextColor(color);
    }
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
    for (auto i = 0; i < userListWidget->count(); ++i) {
        auto item = userListWidget->item(i);
        auto name = item->text().toStdString();
        if (!chatClient->chatroom()->chateeExists(name)) {
            delete item;
        }
    }

    // add chatees to the widget list
    // update user background color on the list
    for (auto const& entry : chatClient->chatroom()->map()) {
        auto name = QString::fromStdString(entry.first);
        auto items = userListWidget->findItems(name,
                                               Qt::MatchExactly);
        QListWidgetItem* item;

        if (items.isEmpty()) {
            item = new QListWidgetItem();
            item->setText(name);
            userListWidget->addItem(item);
        }
        else {
            item = items[0];
        }

        if(entry.second->user().presence() == UserPresence::ONLINE) {
            item->setBackground(QBrush(QColor(0, 220, 0, 180))); // green
        }
        else if(entry.second->user().presence() == UserPresence::AWAY) {
            item->setBackground(QBrush(QColor(220, 220, 0, 180))); // yellow
        }
        else if(entry.second->user().presence() == UserPresence::BUSY) {
            item->setBackground(QBrush(QColor(220, 0, 0, 180))); // red
        }
    }
}

void ChatDialog::updateStatus(const QString& currentSelection) {
    if(currentSelection == "Online") {
        chatClient->updatePresence(UserPresence::ONLINE);
    }
    else if(currentSelection == "Away") {
        chatClient->updatePresence(UserPresence::AWAY);
    }
    else if(currentSelection == "Busy") {
        chatClient->updatePresence(UserPresence::BUSY);
    }
}

void ChatDialog::returnPressed() const {
    auto text = lineEdit->text();
    if (text.isEmpty())
        return;

    if (text.startsWith('/')) {
        QString message;

        if (text.startsWith("/w")) { // whisper
            auto list = text.split(' ');

            if (list.size() < 3)
                message = tr("* sending a whisper failed, wrong number of arguments");
            else if(list[1].toStdString() == chatClient->name())
                message = tr("* can't send a whisper to yourself");
            else {
                auto target = list[1].toStdString();
                list.removeFirst();
                list.removeFirst();
                chatClient->sendMessage(list.join(" ").toStdString(), target);
            }
        }
        else { // command
            auto success = chatClient->sendCommand(text.toStdString());
            if (success)
                message = tr("* sending command: %1").arg(text);
            else
                message = tr("* commmand unrecognized, try /help");
        }

        if (!message.isEmpty()) {
            qDebug() << "command" << text;
            auto color = textEdit->textColor();
            textEdit->setTextColor(QColor(30, 150, 0, 170));
            textEdit->append(message);
            textEdit->setTextColor(color);
        }
    }
    else {
        qDebug() << "message" << text;
        chatClient->sendMessage(text.toStdString());
    }

    lineEdit->clear();
}

void ChatDialog::appendWhisper(QListWidgetItem* item) {
    if(item->text().toStdString() != chatClient->name()) {
        lineEdit->setText(tr("/w %1 ").arg(item->text()));
    }
}

void ChatDialog::showInformation() {
    if (userListWidget->count() == 1) {
        QMessageBox::information(this,
                                 tr("Chat"),
                                 tr("Launch several instances of this "
                                 "program on your local network and "
                                 "start chatting!"));
    }
}

void ChatDialog::loginToChat(const QString& address, quint16 port, const QString& name) {
    loginDialog->hide();
    if(chatClient->login(address, port, name)) { // if login succeeded show the chat window
        this->showChat();
    }
    else {
        QMessageBox::information(this,
                                 tr("Chat connection failed"),
                                 tr("Connecting to the server has failed. "
                                    "Make sure that the server is online"));
    }
}

void ChatDialog::showChat() {
    setupUi(this);
    this->show();

    lineEdit->setFocusPolicy(Qt::StrongFocus);
    textEdit->setFocusPolicy(Qt::NoFocus);
    textEdit->setReadOnly(true);
    userListWidget->setFocusPolicy(Qt::NoFocus);

    connect(chatClient, SIGNAL(chatMessageSignal(QString, QString, QString)),
            this, SLOT(appendMessage(QString, QString, QString)));
    connect(chatClient, SIGNAL(chatInfoSignal(QString)),
            this, SLOT(appendInfo(QString)));
    connect(chatClient, SIGNAL(chatMotdSignal(QString)),
            this, SLOT(appendMotd(QString)));
    connect(chatClient, SIGNAL(chatRefreshListSignal()),
            this, SLOT(refreshList()));
    connect(lineEdit, SIGNAL(returnPressed()),
            this, SLOT(returnPressed()));
    connect(userListWidget, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(appendWhisper(QListWidgetItem*)));
    connect(userStatus, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(updateStatus(QString)));

    tableFormat.setBorder(0);
}

} // SimpleChat namespace