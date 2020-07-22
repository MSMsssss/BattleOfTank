#include "projectileactor.h"
#include "model/tankactor.h"
#include "model/blockactor.h"
#include "model/projectileactor.h"

ProjectileActor::ProjectileActor(bool isDynamic) :
    Actor(isDynamic)
{

}

QDataStream& operator<<(QDataStream &outStream, const ProjectileActor &actor)
{
    actor.out(outStream);
    return outStream;
}

QDataStream& operator>>(QDataStream &inStream, ProjectileActor &actor)
{
    actor.in(inStream);
    return inStream;
}

int ProjectileActor::damageValue() const
{
    return _damageValue;
}

void ProjectileActor::setDamageValue(int damageValue)
{
    _damageValue = damageValue;
}

PlayerController *ProjectileActor::controller() const
{
    return _controller;
}

void ProjectileActor::setController(PlayerController *controller)
{
    _controller = controller;
}

void ProjectileActor::in(QDataStream &inStream)
{
    inStream >> _type;
    Actor::in(inStream);
}

void ProjectileActor::out(QDataStream &outStream) const
{
    outStream << _type;
    Actor::out(outStream);
}

ProjectileActor::ProjectileType ProjectileActor::type() const
{
    return _type;
}

void ProjectileActor::setType(const ProjectileType &type)
{
    _type = type;
}
