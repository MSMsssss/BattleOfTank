#include "gamerunning.h"
#include "gameroom.h"
#include "config.h"
#include "model/playercontroller.h"
#include "model/actor.h"
#include "model/tankactor.h"
#include "model/blockactor.h"
#include "model/projectileactor.h"
#include "model/squarecomponent.h"
#include <QCoreApplication>
#include <QTimer>
#include <QDataStream>
#include <QByteArray>

GameRunning::GameRunning(QObject *parent) : QObject(parent)
{
    frameInterval = 60;
    oneRoundTime = 10 * 60 * 1000;

    frameQTimer = new QTimer(this);
    //frameQTimer->setTimerType(Qt::PreciseTimer);
    connect(frameQTimer, &QTimer::timeout, this, &GameRunning::tick);

    roundQTimer = new QTimer(this);
    connect(roundQTimer, &QTimer::timeout, this, &GameRunning::handleRoundTimerOut);

    connect(this, &GameRunning::gameStart, this, &GameRunning::handleGameStart);
    connect(this, &GameRunning::gameOver, this, &GameRunning::handleGameOver);

    recycleBin.resize(100);
    killMessage.resize(100);
    particleMessage.resize(100);

    resetMap();
}

int GameRunning::getFrameInterval() const
{
    return frameInterval;
}

void GameRunning::setFrameInterval(int value)
{
    frameInterval = value;
}

int GameRunning::getOneRoundTime() const
{
    return oneRoundTime;
}

void GameRunning::setOneRoundTime(int value)
{
    oneRoundTime = value;
}

bool GameRunning::containsPlayer(const QString& playerName)
{
    return playersHash.contains(playerName);
}

PlayerController *GameRunning::getPlayerController(const QString &playerName)
{
    if(!playersHash.contains(playerName))
        return nullptr;

    return playersHash[playerName];
}

bool GameRunning::containsActor(Actor *actor)
{
    if(actor == nullptr)
        return false;

    return dynamicActorSet.contains(actor) || staticActorSet.contains(actor);
}

void GameRunning::addActor(Actor *actor, bool isDynamic)
{
    if(actor == nullptr || containsActor(actor))
        return;

    if(isDynamic)
        dynamicActorSet.insert(actor);
    else
        staticActorSet.insert(actor);
}

bool GameRunning::removeActor(Actor *actor)
{
    bool exists = dynamicActorSet.remove(actor);
    if(!exists)
        exists = staticActorSet.remove(actor);

    return exists;
}

void GameRunning::handleGameStart()
{
    if(!isRunning){
        frameQTimer->setInterval(frameInterval);
        frameQTimer->start();

        roundQTimer->setInterval(oneRoundTime);
        roundQTimer->start();

        isRunning = true;
    }
}

void GameRunning::handleGameOver()
{
    if(isRunning)
    {
        resetMap();
        frameQTimer->stop();
        roundQTimer->stop();

        isRunning = false;
    }
}

//  碰撞检测
void GameRunning::CollisionCheck()
{
    //  动态Actor与动态Actor之间的检测
    for(auto actorPtr = dynamicActorSet.begin(); actorPtr != dynamicActorSet.end(); actorPtr++)
    {
        if((*actorPtr)->isDestroyed() || !(*actorPtr)->isCollisionable())
            continue;

        auto otherActorPtr = actorPtr;
        for(otherActorPtr++; otherActorPtr != dynamicActorSet.end(); otherActorPtr++)
        {
            if((*otherActorPtr)->isDestroyed() || !(*otherActorPtr)->isCollisionable())
                continue;

            //  如果发生碰撞，则调用onHit函数进行处理
            if((*actorPtr)->isCollision(*otherActorPtr))
            {
                (*actorPtr)->onHit(*otherActorPtr);
                (*otherActorPtr)->onHit(*actorPtr);
            }
        }
    }

    //  动态Actor与静态Actor之间的检测
    for(auto actorPtr = dynamicActorSet.begin(); actorPtr != dynamicActorSet.end(); actorPtr++)
    {
        if((*actorPtr)->isDestroyed() || !(*actorPtr)->isCollisionable())
            continue;

        for(auto otherActorPtr = staticActorSet.begin(); otherActorPtr != staticActorSet.end(); otherActorPtr++)
        {
            if((*otherActorPtr)->isDestroyed() || !(*otherActorPtr)->isCollisionable())
                continue;

            //  如果发生碰撞，则调用onHit函数进行处理
            if((*actorPtr)->isCollision(*otherActorPtr))
            {
                (*actorPtr)->onHit(*otherActorPtr);
                (*otherActorPtr)->onHit(*actorPtr);
            }
        }
    }
}

void GameRunning::sendData()
{
    //  发送游戏局势信息包
    {
        QByteArray packet;
        QDataStream out(&packet, QIODevice::WriteOnly);

        out << Config::GameInfoPacket;
        out << roundQTimer->remainingTime();

        for(PlayerController* controller : playersHash)
        {
            emit packetToSend(controller->playerName(), packet);
        }
    }

    //  发送玩家数据包
    {
        QByteArray packet;
        QDataStream out(&packet, QIODevice::WriteOnly);

        out << Config::PlayerInfoListPacket;
        out << playersHash.size();

        for(PlayerController* controller : playersHash)
        {
            out << *controller;
        }

        for(PlayerController* controller : playersHash)
        {
            emit packetToSend(controller->playerName(), packet);
        }
    }

    //  发送子弹数据包
    {
        QByteArray packet;
        QDataStream out(&packet, QIODevice::WriteOnly);

        out << Config::ProjectileInfoListPacket;
        out << int(0);

        int num = 0;
        for(Actor* actor : dynamicActorSet)
        {
            ProjectileActor* bullet;
            if((bullet = qobject_cast<ProjectileActor*>(actor)) != nullptr)
            {
                out << *bullet;
                ++num;
            }
        }

        out.device()->seek(sizeof(Config::ProjectileInfoListPacket));
        out << num;

        for(PlayerController* controller : playersHash)
        {
            emit packetToSend(controller->playerName(), packet);
        }
    }

    //  发送墙体数据包
    {
        QByteArray packet;
        QDataStream out(&packet, QIODevice::WriteOnly);

        out << Config::BlockInfoListPacket;
        out << int(0);

        int num = 0;
        for(Actor* actor : staticActorSet)
        {
            BlockActor* block;
            if((block = qobject_cast<BlockActor*>(actor)) != nullptr)
            {
                if(block->_isStatusChanged)
                {
                    out << *block;
                    ++num;
                }
            }
        }

        out.device()->seek(sizeof(Config::BlockInfoListPacket));
        out << num;

        if(num > 0)
        {
            for(PlayerController* controller : playersHash)
            {
                emit packetToSend(controller->playerName(), packet);
            }
        }
    }

    //  发送击杀列表
    if(!killMessage.isEmpty())
    {
        QByteArray packet;
        QDataStream out(&packet, QIODevice::WriteOnly);

        out << Config::KillListPacket;
        out << killMessage.size();

        for(const auto &message : killMessage)
        {
            out << message.first << message.second;
        }

        for(PlayerController* controller : playersHash)
        {
            emit packetToSend(controller->playerName(), packet);
        }
    }

    //  发送粒子发射信息
    if(!particleMessage.isEmpty())
    {
        QByteArray packet;
        QDataStream out(&packet, QIODevice::WriteOnly);

        out << Config::ParticleEmitPacket;
        out << particleMessage.size();

        for(const auto& message : particleMessage)
        {
            out << message.first << message.second;
        }

        for(PlayerController* controller : playersHash)
        {
            emit packetToSend(controller->playerName(), packet);
        }
    }
}

QSize GameRunning::mapSize() const
{
    return {4000, 2000};
}

void GameRunning::setRoom(GameRoom *value)
{
    room = value;
}

void GameRunning::resetMap()
{
    for(Actor* actor : staticActorSet)
    {
        actor->deleteLater();
    }
    staticActorSet.clear();

    for(int x = 400; x < mapSize().width() - BlockActor::InitialSize.width(); x += 250)
    {
        for(int y = 0; y < mapSize().height() - BlockActor::InitialSize.height(); y += 400)
        {
            BlockActor* actor = new BlockActor(this, this, false);
            actor->setSize(BlockActor::InitialSize);
            actor->setLocation({x, y});
            //  设置碰撞组件
            actor->setCollisionComponent(new SquareComponent(actor->width(), actor->height(), actor));
        }
    }
}

void GameRunning::tick()
{
    //  帧初始化
    tickReset();

    auto playerOperationQueue = room->getOperation();
    for(const auto& temp : playerOperationQueue)
    {
        if(playersHash.contains(temp.first))
            playersHash[temp.first]->applyOperation(temp.second);
    }

    //  运行每个Actor的帧函数
    for(Actor* t : dynamicActorSet.values() + staticActorSet.values())
    {
        t->tick();
    }

    //   碰撞检测
    CollisionCheck();

    //  发送网络数据包
    sendData();

    //  Actor回收
    for(Actor* actor : recycleBin)
    {
        if(removeActor(actor))
        {
            actor->deleteLater();
        }
    }
}

void GameRunning::tickReset()
{
    recycleBin.clear();
    killMessage.clear();
    particleMessage.clear();
}

void GameRunning::handleRoundTimerOut()
{
    emit gameOver();
}

void GameRunning::handlePlayerJoinGame(QString playerName)
{
    PlayerController* controller = new PlayerController();
    controller->setPlayerName(playerName);
    controller->setGameRunning(this);

    if(redCompanyNum > blueCompanyNum)
    {
        controller->setCompany(PlayerController::BLUE_COMPANY);
        ++blueCompanyNum;
    }
    else
    {
        controller->setCompany(PlayerController::RED_COMPANY);
        ++redCompanyNum;
    }

    controller->revive();
    playersHash.insert(playerName, controller);

    {
        QByteArray packet;
        QDataStream(&packet, QIODevice::WriteOnly) << Config::MapInfoPacket
                                                   << mapSize();

        emit packetToSend(playerName, packet);
    }

    //发送初始墙体信息
    {
        QByteArray packet;
        QDataStream out(&packet, QIODevice::WriteOnly);

        out << Config::BlockInfoListPacket;
        out << int(0);

        int num = 0;
        for(Actor* actor : staticActorSet)
        {
            BlockActor* block;
            if((block = qobject_cast<BlockActor*>(actor)) != nullptr)
            {
                out << *block;
                ++num;
            }
        }

        out.device()->seek(sizeof(Config::BlockInfoListPacket));
        out << num;

        emit packetToSend(playerName, packet);
    }

    if(playersHash.size() >= minStartPeopleNum)
    {
        emit gameStart();
    }
}

void GameRunning::handlePlayerQuitGame(QString playerName)
{
    if(!playersHash.contains(playerName))
    {
        return;
    }

    PlayerController* controller = playersHash[playerName];

    //  减去对应的阵营人数
    if(controller->company() == PlayerController::RED_COMPANY)
        --redCompanyNum;
    else
        --blueCompanyNum;

    //  删掉一切与玩家相关的发射体
    for(Actor* t : dynamicActorSet.values() + staticActorSet.values())
    {
        ProjectileActor* actor = qobject_cast<ProjectileActor*>(t);
        if(actor && actor->controller() == controller)
        {
            removeActor(actor);
        }
    }

    // 删掉玩家坦克
    if(controller->tank() != nullptr)
        removeActor(controller->tank());

    //  删掉玩家控制器
    playersHash.remove(playerName);
    controller->deleteLater();

    //  当玩家人数小于2人时，结束游戏
    if(playersHash.size() < minStartPeopleNum)
    {
        emit gameOver();
    }
}

void GameRunning::handleKill(QString killerName, QString slainName)
{
    killMessage.push_back({killerName, slainName});
}

void GameRunning::handleParticleEmit(QPoint location, GameRunning::ParticleSystem::ParticleType type)
{
    particleMessage.push_back({location, type});
}

void GameRunning::recycleActor(Actor *actor)
{
    if(actor != nullptr)
        recycleBin.push_back(actor);
}
