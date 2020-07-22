#include "model/playercontroller.h"
#include "model/tankactor.h"
#include "playeroperation.h"

QDataStream& operator>>(QDataStream &inStream, PlayerController &controller)
{
    inStream >> controller._playerName >> controller._killNum >> controller._deathNum
             >> controller._isDead >> controller._company >> controller._scores;

    if(!controller.isDead())
        inStream >> controller._tank;

    return inStream;
}

QDataStream& operator<<(QDataStream &outStream, const PlayerController &controller)
{
    outStream << controller._playerName << controller._killNum << controller._deathNum
             << controller._isDead << controller._company << controller._scores;

    if(!controller.isDead())
        outStream << controller._tank;

    return outStream;
}

PlayerController::PlayerController()
{

}

QString PlayerController::playerName() const
{
    return _playerName;
}

void PlayerController::setPlayerName(const QString &playerName)
{
    _playerName = playerName;
}

int PlayerController::killNum() const
{
    return _killNum;
}

void PlayerController::setKillNum(int killNum)
{
    _killNum = killNum;
}

int PlayerController::deathNum() const
{
    return _deathNum;
}

void PlayerController::setDeathNum(int deathNum)
{
    _deathNum = deathNum;
}

int PlayerController::scores() const
{
    return _scores;
}

void PlayerController::setScores(int scores)
{
    _scores = scores;
}

int PlayerController::company() const
{
    return _company;
}

void PlayerController::setCompany(int company)
{
    _company = company;
}

int PlayerController::initialTankSpeed() const
{
    return _initialTankSpeed;
}

void PlayerController::setInitialTankSpeed(int initialTankSpeed)
{
    _initialTankSpeed = initialTankSpeed;
}

int PlayerController::speedUpRate() const
{
    return _speedUpRate;
}

void PlayerController::setSpeedUpRate(int speedUpRate)
{
    _speedUpRate = speedUpRate;
}

bool PlayerController::isDead() const
{
    return _isDead;
}

TankActor PlayerController::tank() const
{
    return _tank;
}

const TankActor *PlayerController::tankPtr() const
{
    return &_tank;
}

TankActor &PlayerController::tankRef()
{
    return _tank;
}

const TankActor &PlayerController::tankRefConst() const
{
    return _tank;
}

QPoint PlayerController::deathLocation() const
{
    return _deathLocation;
}

void PlayerController::setDeathLocation(const QPoint &deathLocation)
{
    _deathLocation = deathLocation;
}
