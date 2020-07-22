#include "model/tankactor.h"
#include "model/blockactor.h"
#include "model/projectileactor.h"
#include "model/playercontroller.h"
#include "gamerunning.h"
#include "squarecomponent.h"
#include <QTimer>

const QSize TankActor::InitialSize = {60, 60};

TankActor::TankActor(QObject *parent, GameRunning *gameRunning, bool isDynamic) :
    Actor(parent, gameRunning, isDynamic)
{
    _machineGunTimer = new QTimer(this);
    _armorPiercingTimer = new QTimer(this);

    connect(_machineGunTimer, &QTimer::timeout, [this]()
    {
        if(this->machineGunBulletNum() > 0)
            this->_machineGunIsReady = true;
    });

    connect(_armorPiercingTimer, &QTimer::timeout, [this]()
    {
        if(this->armorPiercingBulletNum() > 0)
            this->_armorPiercingIsReady = true;
    });

    _machineGunTimer->start(_machineGunReloadTime);
    _armorPiercingTimer->start(_armorPiercingReloadTime);

    this->setSize(TankActor::InitialSize);
}

QDataStream& operator<<(QDataStream &outStream, const TankActor &actor)
{
    actor.out(outStream);
    return outStream;
}

QDataStream& operator>>(QDataStream &inStream, TankActor &actor)
{
    actor.in(inStream);
    return inStream;
}

int TankActor::health() const
{
    return _health;
}

void TankActor::setHealth(int health)
{
    _health = health;
}

int TankActor::armorValue() const
{
    return _armorValue;
}

void TankActor::setArmorValue(int armorValue)
{
    _armorValue = armorValue;
}

ProjectileActor::ProjectileType TankActor::projectileType() const
{
    return _projectileType;
}

void TankActor::setProjectileType(const ProjectileActor::ProjectileType &projectileType)
{
    _projectileType = projectileType;
}

void TankActor::switchProjectile()
{
    if(_projectileType == ProjectileActor::MachineGunProjectile)
    {
        _projectileType = ProjectileActor::ArmorPiercingProjectile;
    }
    else
    {
        _projectileType = ProjectileActor::MachineGunProjectile;
    }
}

int TankActor::bulletSpeedAddition() const
{
    return _bulletSpeedAddition;
}

void TankActor::setBulletSpeedAddition(int bulletSpeedAddition)
{
    _bulletSpeedAddition = bulletSpeedAddition;
}

int TankActor::bulletDamageAddition() const
{
    return _bulletDamageAddition;
}

void TankActor::setBulletDamageAddition(int bulletDamageAddition)
{
    _bulletDamageAddition = bulletDamageAddition;
}

PlayerController *TankActor::controller() const
{
    return _controller;
}

void TankActor::setController(PlayerController *controller)
{
    _controller = controller;
}

void TankActor::in(QDataStream &inStream)
{
    inStream >> _health >> _armorValue >> _machineGunBulletNum
             >> _armorPiercingBulletNum >> _armorPiercingIsReady
             >> _speedUpRate >> _projectileType;

    Actor::in(inStream);
}

void TankActor::out(QDataStream &outStream) const
{
    outStream << _health << _armorValue << _machineGunBulletNum
             << _armorPiercingBulletNum << _armorPiercingIsReady
             << _speedUpRate << _projectileType;

    Actor::out(outStream);
}

int TankActor::speedUpRate() const
{
    return _speedUpRate;
}

void TankActor::setSpeedUpRate(int speedUpRate)
{
    _speedUpRate = speedUpRate;
}

Vector2D TankActor::velocity() const
{
    return _baseVelocity * _speedUpRate;
}

void TankActor::setIsAutoFire(bool isAutoFire)
{
    _isAutoFire = isAutoFire;
}

int TankActor::machineGunBulletNum() const
{
    return _machineGunBulletNum;
}

int TankActor::armorPiercingBulletNum() const
{
    return _armorPiercingBulletNum;
}

bool TankActor::isAutoFire() const
{
    return _isAutoFire;
}

void TankActor::tick()
{
    if(_isMovable)
    {
        this->setOldLocation(this->location());
        this->setLocation(this->location() + this->velocity());

        if(reachBoundary())
            this->setLocation(this->oldLocation());
    }
    
    if(_isAutoFire && _projectileType == ProjectileActor::MachineGunProjectile)
    {
        fire();
    }
}

void TankActor::onHit(Actor *otherActor)
{
    if(otherActor == nullptr || otherActor == this)
        return;

    TankActor* tankActor = nullptr;
    ProjectileActor* projectileActor = nullptr;
    BlockActor* blockActor = nullptr;

    if((tankActor = qobject_cast<TankActor*>(otherActor)) != nullptr)
    {
        setLocation(oldLocation());
    }
    else if((projectileActor = qobject_cast<ProjectileActor*>(otherActor)) != nullptr)
    {
        getDamage(projectileActor->controller(), projectileActor->damageValue());
    }
    else if((blockActor = qobject_cast<BlockActor*>(otherActor)) != nullptr)
    {
        setLocation(oldLocation());
    }
    else;
}

void TankActor::fire()
{
    if((_projectileType == ProjectileActor::MachineGunProjectile && !_machineGunIsReady)
       || (_projectileType == ProjectileActor::ArmorPiercingProjectile && !_armorPiercingIsReady))
        return;

    ProjectileActor* bullet = new ProjectileActor(nullptr, _gameRunning, true);
    bullet->setController(_controller);

    switch (_projectileType) {
    case ProjectileActor::MachineGunProjectile:
    {
        bullet->setBaseVelocity(this->faceDirection() * (ProjectileActor::BaseProjectileSpeed * bulletSpeedAddition()));
        bullet->setDamageValue(ProjectileActor::BaseDamageValue * 2 * bulletDamageAddition());
        bullet->setSize(ProjectileActor::MachineGunInitialSize);
        bullet->setType(ProjectileActor::MachineGunProjectile);
        --_machineGunBulletNum;
        _machineGunIsReady = false;
        break;
    }
    case ProjectileActor::ArmorPiercingProjectile:
    {
        bullet->setBaseVelocity(this->faceDirection() * (ProjectileActor::BaseProjectileSpeed * 2 * bulletSpeedAddition()));
        bullet->setDamageValue(ProjectileActor::BaseDamageValue * 10 * bulletDamageAddition());
        bullet->setSize(ProjectileActor::ArmorInitialSize);
        bullet->setType(ProjectileActor::ArmorPiercingProjectile);
        --_armorPiercingBulletNum;
        _armorPiercingIsReady = false;
        break;
    }
    default:
        throw "not have this projectile";
        break;
    }

    bullet->setRotation(this->rotation());
    bullet->setLocation({this->x() + (this->width() - bullet->width()) / 2,
                         this->y() + (this->height() - bullet->height()) / 2});
    bullet->setOldLocation(this->location());
    bullet->setCollisionComponent(new SquareComponent(0, 0, bullet));
}

void TankActor::getDamage(PlayerController* attacker, int damageValue)
{
    if(attacker == this->_controller || attacker == nullptr)
        return;

    if(attacker->company() == this->_controller->company())
        return;

    if(damageValue <= _armorValue)
    {
        _armorValue -= damageValue;
    }
    else
    {
        _health = _health - (damageValue - _armorValue);
        _armorValue = 0;
    }

    if(_health <= 0)
    {
        this->DestroyLater();
        this->_controller->die();
        attacker->setKillNum(attacker->killNum() + 1);
        _gameRunning->handleKill(attacker->playerName(), _controller->playerName());
        _gameRunning->handleParticleEmit(this->location(), GameRunning::ParticleSystem::TankBomb);
    }
}
