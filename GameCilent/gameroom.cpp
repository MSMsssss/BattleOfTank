#include "gameroom.h"

GameRoom::GameRoom(QObject *parent) : QObject(parent)
{

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
