#pragma once

#include <QtNetwork>

#include <client/ChatClient.h>

namespace SimpleChat {

class User;
class TcpChatConnection;

class TcpChatClient : public QObject, public ChatClient {
    Q_OBJECT
public:
    TcpChatClient();

    virtual bool login(const QString& address, quint16 port, const QString& name);
    virtual void logout();

protected:
    bool sendAnyMessage(std::unique_ptr<AbstractMessage> message) override;
    bool isConnected() override;
    ChatConnection* connection() override;

    void chatMotdChanged(const std::string& motd) override;
    void chatInfoReceived(const std::string& info) override;
    void chatMessageReceived(const std::string& text, const std::string& from, const std::string& target) override;
    void refreshChateeList() override;

signals:
    void chatMessageSignal(const QString& text, const QString& from, const QString& target);
    void chatInfoSignal(const QString& text);
    void chatMotdSignal(const QString& motd);
    void chatRefreshListSignal();

private slots:
    void handleUntypedMessage(const QString& serializedData);
    void connectionLost();

    void displayError(QAbstractSocket::SocketError socketError) const;

private:
    TcpChatConnection* serverConnection_;
    QHostAddress serverAddress_;
    quint16 serverPort_;

    virtual bool connectToServer() override;

    void join() override;
    void requestUserList() override;

    QString format(const std::string& info);
    QString format(const User& user, const char* info);
    QString format(const User& user, const std::string& info);
    QString format(const User& user, const QString& info);
};

} // SimpleChat namespace
