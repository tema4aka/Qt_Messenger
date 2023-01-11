#ifndef MESSENGERSERVER_H
#define MESSENGERSERVER_H
#include <QTcpServer>
#include "messengersocket.h"

class MessengerServer : public QTcpServer
{
public:
    MessengerServer(QObject *parent = nullptr);
    bool startServer(quint16 port);
protected:
    void incomingConnection(qintptr handle) override;

private:
    QList<MessengerSocket *> mSockets;
};

#endif // MESSENGERSERVER_H
