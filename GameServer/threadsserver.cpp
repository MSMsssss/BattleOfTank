#include "threadsserver.h"
#include "packettcpsocket.h"
#include "config.h"
#include <QThread>
#include <QDataStream>

ThreadsServer::ThreadsServer(QObject *parent) : QTcpServer(parent)
{

}

void ThreadsServer::incomingConnection(qintptr handle)
{
    PacketTcpSocket* socket = new PacketTcpSocket();
    if(socket->setSocketDescriptor(handle))
    {
        QThread* socketThread = new QThread(this);
        connect(socket, &PacketTcpSocket::disconnected, socketThread, &QThread::quit);
        connect(socket, &PacketTcpSocket::errorOccurred, socketThread, &QThread::quit);
        connect(socket, &PacketTcpSocket::disconnected, this, &ThreadsServer::loginOut);
        connect(socket, &PacketTcpSocket::errorOccurred, this, &ThreadsServer::loginOut);

        connect(socket, SIGNAL(newPacket(QByteArray)), this, SLOT(handlePacket(QByteArray)));
        connect(this, SIGNAL(packetToSend(PacketTcpSocket*, QByteArray)), socket, SLOT(sendPacket(PacketTcpSocket*, QByteArray)));

        connect(socketThread, &QThread::finished, socket, &PacketTcpSocket::deleteLater);
        connect(socketThread, &QThread::finished, socketThread, &QThread::deleteLater);

        socket->moveToThread(socketThread);
        socketThread->start();
    }
    else
    {
        delete socket;
    }
}

int ThreadsServer::handleLogin(const QString &playerName, const QString &password)
{
    if(nameToSocket.contains(playerName))
        return Config::RepeatLogin;

    if(Config::TestMode)
    {
        if(!Config::playerAccountList.contains(playerName))
            return Config::PlayerNameError;

        if(Config::playerAccountList[playerName] != password)
            return Config::PasswordError;

        return Config::LoginSuccess;
    }
    else
    {
        return -1;
    }
}

void ThreadsServer::handlePacket(QByteArray packet)
{
    PacketTcpSocket* socket;
    if((socket = qobject_cast<PacketTcpSocket*>(sender())) != nullptr)
    {
        quint16 type;
        QDataStream in(&packet, QIODevice::ReadOnly);
        in >> type;

        if(type == Config::LoginRequestPacket)
        {
            QString playerName, password;
            in >> playerName >> password;
            int status = handleLogin(playerName, password);

            QByteArray replyPacket;
            QDataStream(&replyPacket, QIODevice::WriteOnly) << Config::LoginReplyPacket
                                                            << status;

            if(status == Config::LoginSuccess)
            {
                nameToSocket.insert(playerName, socket);
                socketToName.insert(socket, playerName);
                emit playerLoginIn(playerName);
            }

            emit packetToSend(socket, replyPacket);
        }
        else
        {
            if(socketToName.contains(socket))
            {
                emit newPacket(socketToName[socket], packet);
            }
        }
    }

}

void ThreadsServer::sendPacket(QString playerName, QByteArray packet)
{
    if(playerName.isEmpty())
    {
        /* 广播该数据包 */
        emit packetToSend(nullptr, packet);
    }

    if(nameToSocket.contains(playerName))
    {
        emit packetToSend(nameToSocket[playerName], packet);
    }
}

void ThreadsServer::loginOut()
{
    PacketTcpSocket* socket;
    if((socket = qobject_cast<PacketTcpSocket*>(sender())) != nullptr)
    {
        if(socketToName.contains(socket))
        {
            QString playerName = socketToName[socket];

            socketToName.remove(socket);
            nameToSocket.remove(playerName);
            emit playerLoginOut(playerName);
        }
    }
}
