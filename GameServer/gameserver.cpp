#include "gameserver.h"
#include "threadsserver.h"
#include "servercontrollerwindow.h"
#include "config.h"
#include "gameroom.h"
#include <QThread>
#include <QDebug>
#include <QDataStream>
#include <QByteArray>

GameServer::GameServer(QObject *parent) : QObject(parent)
{
    initRooms();
    threadsServer = new ThreadsServer();
    QThread* serverThread = new QThread();

    connect(serverThread, &QThread::finished, serverThread, &QThread::deleteLater);
    connect(serverThread, &QThread::finished, threadsServer, &ThreadsServer::deleteLater);

    connect(threadsServer, SIGNAL(newPacket(QString, QByteArray)), this, SLOT(handlePacketFromCilent(QString, QByteArray)));
    connect(this, SIGNAL(packetToSend(QString, QByteArray)), threadsServer, SLOT(sendPacket(QString, QByteArray)));

    connect(threadsServer, SIGNAL(playerLoginIn(QString)), this, SLOT(handleLoginIn(QString)));
    connect(threadsServer, SIGNAL(playerLoginOut(QString)), this, SLOT(handleLoginOut(QString)));

    threadsServer->listen(Config::serverAddress, Config::serverPort);
    threadsServer->moveToThread(serverThread);
    serverThread->start();
}

GameServer::~GameServer()
{

}

int GameServer::run()
{
    if(Config::TestMode)
        qDebug() << "Main Thread" << QThread::currentThread();

    ServerControllerWindow w;
    w.show();

    return app->exec();
}

void GameServer::initRooms()
{
    GameRoom* room1 = new GameRoom(this);
    GameRoom* room2 = new GameRoom(this);
    GameRoom* room3 = new GameRoom(this);

    room1->setGameMode(GameMode::ConquestMode);
    room2->setGameMode(GameMode::BreakThroughMode);
    room3->setGameMode(GameMode::TeamDeathmatchMode);

    room1->setGameMap(GameMap::MapOne);
    room2->setGameMap(GameMap::MapTwo);
    room3->setGameMap(GameMap::MapThree);

    room1->setRoomName(QStringLiteral("是兄弟就来砍我！！！"));
    room2->setRoomName(QStringLiteral("朝鲜冷面杀手"));
    room3->setRoomName(QStringLiteral("少林寺驻武当山办事处"));

    room1->setPlayerNumLimit(12);
    room2->setPlayerNumLimit(8);
    room3->setPlayerNumLimit(6);

    roomHash = {
        {room1->getRoomName(), room1},
        {room2->getRoomName(), room2},
        {room3->getRoomName(), room3}
    };


}

void GameServer::handlePacketFromCilent(QString playerName, QByteArray packet)
{
    QDataStream in(&packet, QIODevice::ReadOnly);
    quint16 type;
    in >> type;

    if(type == Config::RoomsInfoRequestPacket)
    {
        QByteArray replyPacket;
        QDataStream out(&replyPacket, QIODevice::WriteOnly);
        out << Config::ReplyRoomsInfoRequestPacket;
        out << roomHash.size();

        for(GameRoom* room: roomHash.values())
        {
            out << *room;
        }

        emit packetToSend(playerName, replyPacket);
    }
    else if (type == Config::RequestJoinRoomPacket)
    {
        QString roomName;
        in >> roomName;
        QByteArray replyPacket;
        QDataStream out(&replyPacket, QIODevice::WriteOnly);
        out << Config::ReplyJoinRoomPacket;

        if(roomHash.contains(roomName))
        {
            if(playerToRoom.contains(playerName))
            {
                out << Config::RepeatJoin;
            }
            else
            {
                if(roomHash[roomName]->isFull())
                {
                    out << Config::RoomIsFull;
                }
                else
                {
                    playerToRoom.insert(playerName, roomHash[roomName]);
                    playerStatus[playerName] = PlayerStatus::GamePlaying;
                    roomHash[roomName]->handlePlayerJoin(playerName);
                    out << Config::JoinSuccess << roomName;
                }
            }
        }
        else
        {
            out << Config::RoomNotExists;
        }

        emit packetToSend(playerName, replyPacket);
    }
    else if(type == Config::PlayerOperationPacket)
    {
        GameRoom* room = playerToRoom[playerName];
        if(room)
        {
            int num = 0;
            in >> num;

            PlayerOperation op;
            for(int i = 0; i < num; ++i)
            {
                in >> op;
                room->addOperation({playerName, op});
            }
        }
    }
}

void GameServer::handlePacketFromGameRunning(QString playerName, QByteArray packet)
{
    emit packetToSend(playerName, packet);
}

void GameServer::handleLoginIn(QString playerName)
{
    if(!playerStatus.contains(playerName))
        playerStatus[playerName] = PlayerStatus::StayInMenu;
}

void GameServer::handleLoginOut(QString playerName)
{
    if(playerStatus.contains(playerName))
    {
        if(playerToRoom.contains(playerName))
        {
            playerToRoom[playerName]->handlePlayerQuit(playerName);
        }

        playerStatus.remove(playerName);
        playerToRoom.remove(playerName);
    }
}
