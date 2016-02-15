#pragma once

#include <memory>

#include "ui_chatdialog.h"

namespace SimpleChat {

class LoginDialog;
class TcpChatClient;

class ChatDialog : public QDialog,
                   private Ui::ChatDialog {
    Q_OBJECT
public:
    explicit ChatDialog(QWidget *parent = nullptr);
    ~ChatDialog();
    void start() const;

private slots:
    void appendMessage(const QString &from, const QString &message, const QString& target = "") const;
    void appendInfo(const QString& info) const;
    void appendMotd(const QString& motd) const;
    void refreshList();

    void returnPressed() const;
    void appendWhisper(QListWidgetItem* item);
    void showInformation();
    void loginToChat(const QString& address, quint16 port, const QString& name);

private:
    LoginDialog* loginDialog;
    TcpChatClient* chatClient;

    QTextTableFormat tableFormat;

    void showChat();
};

} // SimpleChat namespace