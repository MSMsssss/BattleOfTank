#ifndef GAMERUNNING_H
#define GAMERUNNING_H

#include <QObject>
#include <QHash>
#include <QQueue>
#include <QSet>
#include <QVector>
#include "playeroperation.h"

class GameRoom;
class QTimer;
class PlayerController;
class Actor;

class GameRunning : public QObject
{
    Q_OBJECT
public:
    class ParticleSystem{
    public:
        using ParticleType = int;
        static const ParticleType Smoke = 1;
        static const ParticleType Bomb = 2;
        static const ParticleType Fire = 3;
        static const ParticleType TankBomb = 4;
    };

public:
    explicit GameRunning(QObject *parent = nullptr);

    int getFrameInterval() const;
    void setFrameInterval(int value);

    int getOneRoundTime() const;
    void setOneRoundTime(int value);

    inline bool containsPlayer(const QString& playerName);

    PlayerController* getPlayerController(const QString& playerName);

    bool containsActor(Actor* actor);

    //  向GameRunning中注册一个Actor
    void addActor(Actor* actor, bool isDynamic);

    //  从GameRunning中移除一个Actor
    bool removeActor(Actor* actor);

    //  碰撞检测
    void CollisionCheck();

    //  向每个客户端发送数据
    void sendData();

    QSize mapSize() const;

    void setRoom(GameRoom *value);

    void resetMap();

private:
    GameRoom* room = nullptr;
    int frameInterval;  // 单位 ms
    int oneRoundTime;
    int minStartPeopleNum = 1;
    bool isRunning = false;
    QTimer* frameQTimer = nullptr;
    QTimer* roundQTimer = nullptr;

    //  双方玩家控制器
    QHash<QString, PlayerController*> playersHash;  //名字 -> 玩家控制器
    int blueCompanyNum = 0;
    int redCompanyNum = 0;

    //  可移动Actor集合，如坦克，炮弹
    QSet<Actor*> dynamicActorSet;

    //  不可移动Actor集合，如墙体
    QSet<Actor*> staticActorSet;

    //  待删除Actor集合
    QVector<Actor*> recycleBin;

    //  击杀消息
    QVector<QPair<QString, QString>> killMessage;   // first kill second

    //  爆炸队列
    QVector<QPair<QPoint, ParticleSystem::ParticleType>> particleMessage;    //  发射地点和粒子类型

signals:
    void gameOver();
    void gameStart();
    void packetToSend(QString playerName, QByteArray packet);

public slots:
    void handleGameStart();
    void handleGameOver();

    //  逻辑帧
    void tick();
    //  帧重置，在每一帧开始前进行一些必要的重置的工作
    void tickReset();

    void handleRoundTimerOut();
    void handlePlayerJoinGame(QString playerName);
    void handlePlayerQuitGame(QString playerName);
    //void handlePacket(QString playerName, QByteArray packet);

    //  处理击杀信息
    void handleKill(QString killerName, QString slainName);

    //  处理粒子发射
    void handleParticleEmit(QPoint location, ParticleSystem::ParticleType type);

    //  回收Actor
    void recycleActor(Actor* actor);
};

#endif // GAMERUNNING_H
