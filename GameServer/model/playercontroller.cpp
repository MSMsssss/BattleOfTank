#include "model/playercontroller.h"
#include "model/tankactor.h"
#include "model/squarecomponent.h"
#include "playeroperation.h"
#include "gamerunning.h"

QDataStream& operator>>(QDataStream &inStream, PlayerController &controller)
{
    inStream >> controller._playerName >> controller._killNum >> controller._deathNum
             >> controller._isDead >> controller._company >> controller._scores;

    if(!controller.isDead())
        inStream >> *controller.tank();

    return inStream;
}

QDataStream& operator<<(QDataStream &outStream, const PlayerController &controller)
{
    outStream << controller._playerName << controller._killNum << controller._deathNum
             << controller._isDead << controller._company << controller._scores;

    if(!controller.isDead())
        outStream << *controller.tank();

    return outStream;
}

PlayerController::PlayerController(QObject *parent) : QObject(parent)
{

}

void PlayerController::applyOperation(const PlayerOperation &op)
{
    switch (op.operation()) {
    case PlayerOperation::MOVE_FORWARD:
    {
        if(_tank)
            _tank->setBaseVelocity(Vector2D(0, -_initialTankSpeed));
        break;
    }
    case PlayerOperation::MOVE_BACKWARD:
    {
        if(_tank)
            _tank->setBaseVelocity(Vector2D(0, _initialTankSpeed));
        break;
    }
    case PlayerOperation::MOVE_LEFT:
    {
        if(_tank)
            _tank->setBaseVelocity(Vector2D(-_initialTankSpeed, 0));
        break;
    }
    case PlayerOperation::MOVE_RIGHT:
    {
        if(_tank)
            _tank->setBaseVelocity(Vector2D(_initialTankSpeed, 0));
        break;
    }
    case PlayerOperation::MOVE_STOP:
    {
        if(_tank)
            _tank->setBaseVelocity(Vector2D(0, 0));
        break;
    }
    case PlayerOperation::SPEED_UP:
    {
        if(_tank)
            _tank->setSpeedUpRate(_speedUpRate);
        break;
    }
    case PlayerOperation::SPEED_UP_STOP:
    {
        if(_tank)
            _tank->setSpeedUpRate(1);
        break;
    }
    case PlayerOperation::ROTATE:
    {
        if(_tank)
            _tank->setRotation(op.value());
        break;
    }
    case PlayerOperation::PRESS_FIRE_BUTTON:
    {
        if(_tank)
        {
            _tank->fire();
            _tank->setIsAutoFire(true);
        }
        break;
    }
    case PlayerOperation::RELEASE_FIRE_BUTTON:
    {
        if(_tank)
            _tank->setIsAutoFire(false);
        break;
    }
    case PlayerOperation::SELECT_BULLET:
    {
        if(_tank)
            _tank->switchProjectile();
        break;
    }
    case PlayerOperation::REVIVAL:
    {
        this->revive();
        break;
    }
    }
}

QString PlayerController::playerName() const
{
    return _playerName;
}

void PlayerController::setPlayerName(const QString &playerName)
{
    _playerName = playerName;
}

GameRunning *PlayerController::gameRunning() const
{
    return _gameRunning;
}

void PlayerController::setGameRunning(GameRunning *gameRunning)
{
    _gameRunning = gameRunning;
}

TankActor *PlayerController::tank() const
{
    return _tank;
}

void PlayerController::setTank(TankActor *tank)
{
    _tank = tank;
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

void PlayerController::revive()
{
    if(_gameRunning == nullptr || _playerName.isEmpty() || _tank != nullptr)
        return;


    _isDead = false;
    _tank = new TankActor(nullptr, _gameRunning, true);
    _tank->setController(this);
    _tank->setSize(TankActor::InitialSize);
    _tank->setLocation({0, 0});
    _tank->setCollisionComponent(new SquareComponent(0, 0, _tank));
}

void PlayerController::die()
{
    _isDead = true;
    _deathNum += 1;
    _tank = nullptr;
}
