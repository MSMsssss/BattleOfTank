#ifndef GAMEROOM_H
#define GAMEROOM_H

#include <QObject>
#include <QString>
#include <QDataStream>
#include "gamemode.h"
#include "gamemap.h"

class GameRoom : public QObject
{

    Q_OBJECT

public:
    explicit GameRoom(QObject *parent = nullptr);
    void run();

    friend QDataStream& operator>>(QDataStream& in, GameRoom& room);
    friend QDataStream& operator<<(QDataStream& out, const GameRoom& room);

    int getPlayerNumLimit() const;
    void setPlayerNumLimit(int value);

    QString getRoomName() const;
    void setRoomName(const QString &value);

    int getPlayerNum() const;

    GameMode::ModeType getGameMode() const;
    void setGameMode(const GameMode::ModeType &value);

    GameMap::MapType getGameMap() const;
    void setGameMap(const GameMap::MapType &value);

private:
    QString roomName = "";
    int playerNumLimit = 0;
    int playerNum = 0;
    GameMode::ModeType gameMode = 0;
    GameMap::MapType gameMap = 0;

signals:

};



#endif // GAMEROOM_H
