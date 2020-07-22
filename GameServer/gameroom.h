#ifndef GAMEROOM_H
#define GAMEROOM_H

#include <QObject>
#include <QString>
#include <QDataStream>
#include <QThread>
#include <QMutex>
#include "gamemode.h"
#include "gamemap.h"
#include "playeroperation.h"

class GameRoom : public QObject
{

    Q_OBJECT

public:
    explicit GameRoom(QObject *parent = nullptr);

    friend QDataStream& operator>>(QDataStream& in, GameRoom& room);
    friend QDataStream& operator<<(QDataStream& out, const GameRoom& room);

    int getPlayerNumLimit() const;
    void setPlayerNumLimit(int value);

    QString getRoomName() const;
    void setRoomName(const QString &value);

    int getPlayerNum() const;
    bool isFull() const;

    GameMode::ModeType getGameMode() const;
    void setGameMode(const GameMode::ModeType &value);

    GameMap::MapType getGameMap() const;
    void setGameMap(const GameMap::MapType &value);

    QVector<QPair<QString, PlayerOperation>> getOperation();
    void addOperation(const QPair<QString, PlayerOperation>& op);

private:
    QString roomName = "";  // 房间名称
    int playerNumLimit = 0; //玩家数量限制
    int playerNum = 0;  //玩家数量
    GameMode::ModeType gameMode = 0;    //游戏模式
    GameMap::MapType gameMap = 0;   //游戏地图
    int roundTime = 20 * 60;    //一局游戏的时间

    QMutex playerOperationQueueMutex;
    QVector<QPair<QString, PlayerOperation>> playerOperationQueue;

signals:
    void playerJoin(QString playerName);
    void playerQuit(QString playerName);

public slots:
    void handlePlayerQuit(const QString& playerName);
    void handlePlayerJoin(const QString& playerName);
};



#endif // GAMEROOM_H
