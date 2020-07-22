#include "blockactor.h"
#include "model/tankactor.h"
#include "model/blockactor.h"
#include "model/projectileactor.h"

const QSize BlockActor::InitialSize = {100, 100};

BlockActor::BlockActor(QObject *parent, GameRunning* gameRunning, bool isDynamic) :
    Actor(parent, gameRunning, isDynamic)
{
    _isStatusChanged = true;
}

QDataStream& operator<<(QDataStream &outStream, const BlockActor &actor)
{
    actor.out(outStream);
    return outStream;
}

QDataStream& operator>>(QDataStream &inStream, BlockActor &actor)
{
    actor.in(inStream);
    return inStream;
}

int BlockActor::health() const
{
    return _health;
}

void BlockActor::setHealth(int health)
{
    _health = health;
}

void BlockActor::in(QDataStream &inStream)
{
    inStream >> _location >> _health >> _size;
}

void BlockActor::out(QDataStream &outStream) const
{
    outStream << _location << _health << _size;
}

void BlockActor::onHit(Actor *otherActor)
{
    ProjectileActor* projectileActor = nullptr;
    
    if((projectileActor = qobject_cast<ProjectileActor*>(otherActor)) != nullptr)
    {
        getDamage(projectileActor->damageValue());
    }
    else;
}

void BlockActor::tick()
{
    _isStatusChanged = false;
}

void BlockActor::getDamage(int damageValue)
{
    _health -= damageValue;
    _isStatusChanged = true;
    if(_health <= 0)
    {
        DestroyLater();
    }
}
