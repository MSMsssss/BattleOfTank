#include "model/tankactor.h"
#include "model/blockactor.h"
#include "model/projectileactor.h"
#include "model/playercontroller.h"
#include <QTimer>

TankActor::TankActor(bool isDynamic) :
    Actor(isDynamic)
{

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

int TankActor::machineGunBulletNum() const
{
    return _machineGunBulletNum;
}

int TankActor::armorPiercingBulletNum() const
{
    return _armorPiercingBulletNum;
}

bool TankActor::armorPiercingIsReady() const
{
    return _armorPiercingIsReady;
}
