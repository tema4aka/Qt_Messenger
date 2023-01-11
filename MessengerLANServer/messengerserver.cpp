#include "messengerserver.h"
#include "messengersocket.h"
#include <QTextStream>
#include <QDebug>

MessengerServer::MessengerServer(QObject *parent)
    : QTcpServer(parent)
{

}

bool MessengerServer::startServer(quint16 port)
{
    return listen (QHostAddress::Any, port);
}

void MessengerServer::incomingConnection(qintptr handle)
{
    qDebug() << "Client conncted with handle: " << handle;
    auto socket = new MessengerSocket(handle, this);
    mSockets << socket;

    for (auto i : mSockets)
    {
        QTextStream T(i);
        T << "Server: connected: " << handle;
        i->flush();
    }

    connect(socket, &MessengerSocket::ReadyRead, [&](MessengerSocket *S)
    {
        qDebug() << "ReadyRead";
        QTextStream T(S);
        auto text = T.readAll();

        for (auto i : mSockets)
        {
            QTextStream K(i);
            K << text;
            i->flush();
        }
    });
    connect(socket, &MessengerSocket::StateChanged, [&](MessengerSocket *S, int ST)
    {
        qDebug() << "StateChanged with handle:" << S->socketDescriptor();
        if (ST == QTcpSocket::UnconnectedState)
        {
            qDebug() << "Unconnected state with handle: " << S->socketDescriptor();
            mSockets.removeOne(S);
            for (auto i : mSockets)
            {
                QTextStream K(i);
                K << "Server: client" << S->socketDescriptor() << "disconnected...";
                i->flush();
            }
        }
    });
}



