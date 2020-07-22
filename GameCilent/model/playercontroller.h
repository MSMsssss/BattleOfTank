#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "tankactor.h"

class PlayerOperation;

class PlayerController
{
public:
    static const int BLUE_COMPANY = 1;
    static const int RED_COMPANY = 2;

public:
    friend QDataStream& operator>>(QDataStream& inStream, PlayerController& controller);
    friend QDataStream& operator<<(QDataStream& outStream, const PlayerController& controller);

    explicit PlayerController();
    void applyOperation(const PlayerOperation& op);

    QString playerName() const;
    void setPlayerName(const QString &playerName);

    int killNum() const;
    void setKillNum(int killNum);

    int deathNum() const;
    void setDeathNum(int deathNum);

    int scores() const;
    void setScores(int scores);

    int company() const;
    void setCompany(int company);

    int initialTankSpeed() const;
    void setInitialTankSpeed(int initialTankSpeed);

    int speedUpRate() const;
    void setSpeedUpRate(int speedUpRate);

    bool isDead() const;

    TankActor tank() const;
    const TankActor* tankPtr() const;
    TankActor& tankRef();
    const TankActor& tankRefConst() const;

    QPoint deathLocation() const;
    void setDeathLocation(const QPoint &deathLocation);

protected:
    QString _playerName = "";
    TankActor _tank;
    int _killNum = 0;
    int _deathNum = 0;
    int _scores = 0;
    int _company = 0;
    int _initialTankSpeed = 50;  // 每秒钟移动的像素点数
    int _speedUpRate = 2;    //  加速度
    bool _isDead = false;
    QPoint _deathLocation = {0, 0};
};

#endif // PLAYERCONTROLLER_H
