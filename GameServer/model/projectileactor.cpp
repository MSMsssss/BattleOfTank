#include "projectileactor.h"
#include "model/tankactor.h"
#include "model/blockactor.h"
#include "model/projectileactor.h"
#include "model/playercontroller.h"
#include "gamerunning.h"

const QSize ProjectileActor::ArmorInitialSize = {36, 6};
const QSize ProjectileActor::MachineGunInitialSize = {18, 9};

ProjectileActor::ProjectileActor(QObject *parent, GameRunning *gameRunning, bool isDynamic) :
    Actor(parent, gameRunning, isDynamic)
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

void ProjectileActor::onHit(Actor *otherActor)
{
    if(otherActor == nullptr || otherActor == this || otherActor == this->controller()->tank())
        return;

    bomb();
}

void ProjectileActor::tick()
{
    if(_isMovable)
    {
        this->setOldLocation(this->location());
        this->setLocation(this->location() + this->baseVelocity());
    }

    if(reachBoundary())
    {
        bomb();
    }
}

void ProjectileActor::bomb()
{
    DestroyLater();
    if(this->_type == ProjectileActor::ArmorPiercingProjectile)
        gameRunning()->handleParticleEmit(location(), GameRunning::ParticleSystem::Bomb);
}
