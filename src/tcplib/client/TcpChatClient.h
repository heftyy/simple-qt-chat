#pragma once

#include <QtNetwork>

#include <client/ChatClient.h>

namespace SimpleChat {

class User;
class TcpChatConnection;

/*!
 * Implements all virtual networking related methods in ChatClient.
 * Provides multiple signals to be used by the GUI.
 *
 * Holds a TcpChatConnection to the server.
 */
class TcpChatClient : public QObject, public ChatClient {
    Q_OBJECT
public:
    explicit TcpChatClient(QObject* parent = nullptr);
    ~TcpChatClient();

    /*!
     * Creates a connection to the server and attempts to join the chat.
     * Joining the server is non-blocking.
     * Returns true if the connection has been established.
     */
    virtual bool login(const QString& address, quint16 port, const QString& name);
    /*!
     * Disconnects from server.
     */
    virtual void logout();

    virtual bool connectToServer() override;
    virtual void join() override;
    virtual void requestUserList() override;

protected:
    virtual bool sendAnyMessage(std::unique_ptr<AbstractMessage> message) override;
    virtual bool isConnected() override;
    virtual ChatConnection* connection() override;

    virtual void chateeJoined(const std::string& name) override;
    virtual void chateeLeft(const std::string& name) override;
    virtual void chatMotdChanged(const std::string& motd) override;    
    virtual void chatInfoReceived(const std::string& info) override;
    virtual void chatMessageReceived(const std::string& text, const std::string& from, const std::string& target) override;
    virtual void refreshChateeList() override;

signals:
    void chateeJoinedSignal(const QString& name);
    void chateeLeftSignal(const QString& name);
    void chatMessageSignal(const QString& text, const QString& from, const QString& target);
    void chatInfoSignal(const QString& text);
    void chatMotdSignal(const QString& motd);
    void chatRefreshListSignal();

private slots:
    virtual void handleUntypedMessage(const QString& serializedData);
    virtual void connectionLost();

    void displayError(QAbstractSocket::SocketError socketError) const;

private:
    TcpChatConnection* serverConnection_;
    QHostAddress serverAddress_;
    quint16 serverPort_;

    QString format(const std::string& info) const;
    QString format(const User& user, const char* info) const;
    QString format(const User& user, const std::string& info) const;
    QString format(const User& user, const QString& info) const;
};

} // SimpleChat namespace
