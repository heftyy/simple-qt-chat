#pragma once

#include <memory>

#include "ui_chatdialog.h"

namespace SimpleChat {

class LoginDialog;
class ChatClient;

class ChatDialog : public QDialog, private Ui::ChatDialog
{
    Q_OBJECT

public:
    ChatDialog(QWidget *parent = 0);
    void start();

public slots:
    void appendMessage(const QString &from, const QString &message);

private slots:
    void returnPressed();
    void newParticipant(const QString &nick);
    void participantLeft(const QString &nick);
    void showInformation();
    void loginToChat(const QString& address, quint16 port, const QString& name);

private:
    std::shared_ptr<LoginDialog> loginDialog;
    std::shared_ptr<ChatClient > chatClient;

    QString myNickName;
    QTextTableFormat tableFormat;

    void showChat(const QString& address, quint16 port, const QString& name);
};

} // SimpleChat namespace