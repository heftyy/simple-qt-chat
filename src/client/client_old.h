#pragma once

#include <QAbstractSocket>
#include <QHash>
#include <QHostAddress>

namespace SimpleChat {

class ChatConnection;

class Client : public QObject {
Q_OBJECT

public:
    Client();

    void sendMessage(const QString& message);
    QString nickName() const;
    bool hasConnection(const QHostAddress& senderIp, int senderPort = -1) const;

signals:
    void newMessage(const QString& from, const QString& message);
    void newParticipant(const QString& nick);
    void participantLeft(const QString& nick);

private slots:
    void newConnection(ChatConnection* connection);
    void connectionError(QAbstractSocket::SocketError socketError);
    void disconnected();
    void readyForUse();

private:
    void removeConnection(ChatConnection* connection);
};

}