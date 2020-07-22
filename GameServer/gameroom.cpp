#include "gameroom.h"
#include "gamerunning.h"
#include "gameserver.h"
#include <QThread>

GameRoom::GameRoom(QObject *parent) : QObject(parent)
{
    GameRunning* gameRunning = new GameRunning();
    gameRunning->setRoom(this);
    GameServer* gameServer = qobject_cast<GameServer*>(parent);

    if(gameServer != nullptr)
    {
        connect(gameRunning, SIGNAL(packetToSend(QString, QByteArray)),
                gameServer, SLOT(handlePacketFromGameRunning(QString, QByteArray)));

        connect(this, SIGNAL(playerJoin(QString)), gameRunning, SLOT(handlePlayerJoinGame(QString)));
        connect(this, SIGNAL(playerQuit(QString)), gameRunning, SLOT(handlePlayerQuitGame(QString)));
    }

    QThread* thread = new QThread();
    gameRunning->moveToThread(thread);

    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(thread, &QThread::finished, gameRunning, &GameRunning::deleteLater);

    thread->start();
}


/* getter and setter */

int GameRoom::getPlayerNumLimit() const
{
    return playerNumLimit;
}

void GameRoom::setPlayerNumLimit(int value)
{
    playerNumLimit = value;
}

QString GameRoom::getRoomName() const
{
    return roomName;
}

void GameRoom::setRoomName(const QString &value)
{
    roomName = value;
}

int GameRoom::getPlayerNum() const
{
    return playerNum;
}

bool GameRoom::isFull() const
{
    return playerNum >= playerNumLimit;
}

GameMode::ModeType GameRoom::getGameMode() const
{
    return gameMode;
}

void GameRoom::setGameMode(const GameMode::ModeType &value)
{
    gameMode = value;
}

GameMap::MapType GameRoom::getGameMap() const
{
    return gameMap;
}

void GameRoom::setGameMap(const GameMap::MapType &value)
{
    gameMap = value;
}

QVector<QPair<QString, PlayerOperation> > GameRoom::getOperation()
{
    playerOperationQueueMutex.lock();
    auto rtn = playerOperationQueue;
    playerOperationQueue.clear();
    playerOperationQueueMutex.unlock();

    return rtn;
}

void GameRoom::addOperation(const QPair<QString, PlayerOperation> &op)
{
    playerOperationQueueMutex.lock();
    playerOperationQueue.push_back(op);
    playerOperationQueueMutex.unlock();
}

void GameRoom::handlePlayerQuit(const QString& playerName)
{
    --playerNum;
    emit playerQuit(playerName);
}

void GameRoom::handlePlayerJoin(const QString &playerName)
{
    ++playerNum;
    emit playerJoin(playerName);
}

QDataStream& operator>>(QDataStream& in, GameRoom& room)
{
    in >> room.roomName >> room.gameMode >> room.playerNum >> room.playerNumLimit >> room.gameMap;
    return in;
}

QDataStream& operator<<(QDataStream& out, const GameRoom& room)
{
    out << room.roomName << room.gameMode << room.playerNum << room.playerNumLimit << room.gameMap;
    return out;
}
