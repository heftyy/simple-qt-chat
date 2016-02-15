#pragma once

#include <memory>

#include "ui_chatdialog.h"

namespace SimpleChat {

class LoginDialog;
class TcpChatClient;

/*!
 * Chat GUI.
 *
 * Uses the chatdialog.ui Qt form.
 */
class ChatDialog : public QDialog,
                   private Ui::ChatDialog {
    Q_OBJECT
public:
    explicit ChatDialog(QWidget *parent = nullptr);
    ~ChatDialog();
    void start() const;

private slots:
    /*!
     * Checks if the message has a target set and adds a message to the chat box.
     * If target is set this methods displays a purple message (whisper) otherwise
     * it displays a regular colored text.
     */
    void appendMessage(const QString &from, const QString &message, const QString& target = "") const;
    /*!
     * Method for adding any information to the chat box except chat messages.
     * Info could be:
     *   - new chatee
     *   - chatee left
     *   - chatee kicked
     *   - chatee muted
     *   - error messages
     *   etc...
     */
    void appendInfo(const QString& info) const;
    /*!
     * Displays a new message of the day.
     */
    void appendMotd(const QString& motd) const;
    /*!
     * Compares the chatee list held in QWidgetList displayed on the right side of the window
     * to the chatee map in Chatroom.
     */
    void refreshList();

    /*!
     * Checks if the lineEdit contains a '/' at the beginning and sends a message using TcpChatClient.
     * if '/':
     *   - /w send a private message
     *   - everything else treated as command (/auth, /mute, /unmute, /kick, /motd etc)
     * else:
     *   - send a global message
     */
    void returnPressed() const;
    /*!
     * add /w <nickname> to lineEdit to send whispers easily
     */
    void appendWhisper(QListWidgetItem* item);
    void showInformation();
    /*!
     * Use the information from the login dialog, hide the login dialog.
     * If logging in is successful show the chat dialog otherwise show a
     * message box with information.
     */
    void loginToChat(const QString& address, quint16 port, const QString& name);

private:
    LoginDialog* loginDialog;
    TcpChatClient* chatClient;

    QTextTableFormat tableFormat;

    void showChat();
};

} // SimpleChat namespace