#include "model/actor.h"
#include "model/squarecomponent.h"
#include "gamerunning.h"
#include <QPoint>
#include <QMatrix>

Actor::Actor(QObject *parent, GameRunning *gameRunning, bool isDynamic) : QObject(parent)
{
    _isDynamic = isDynamic;
    this->setGameRunning(gameRunning);
}

QDataStream& operator<<(QDataStream &outStream, const Actor &actor)
{
    actor.out(outStream);
    return outStream;
}

QDataStream& operator>>(QDataStream &inStream, Actor &actor)
{
    actor.in(inStream);
    return inStream;
}

QPoint Actor::oldLocation() const
{
    return _oldLocation;
}

void Actor::setOldLocation(const QPoint &oldLocation)
{
    _oldLocation = oldLocation;
}

QPoint Actor::location() const
{
    return _location;
}

void Actor::setLocation(const QPoint &location)
{
    _location = location;
    if(_collisionComponent)
        _collisionComponent->setLocation(location);
}

int Actor::x() const
{
    return _location.x();
}

int Actor::y() const
{
    return _location.y();
}

void Actor::setX(int x)
{
    _location.setX(x);
    if(_collisionComponent)
        _collisionComponent->setX(this->x());
}

void Actor::setY(int y)
{
    _location.setY(y);
    if(_collisionComponent)
        _collisionComponent->setY(this->y());
}

QSize Actor::size() const
{
    return _size;
}

void Actor::setSize(const QSize &size)
{
    _size = size;
    if(collisionComponent())
    {
        collisionComponent()->setSize(size);
    }
}

int Actor::width() const
{
    return _size.width();
}

int Actor::height() const
{
    return _size.height();
}

void Actor::setWidth(int width)
{
    _size.setWidth(width);
    if(collisionComponent())
    {
        collisionComponent()->setWidth(width);
    }
}

void Actor::setHeight(int height)
{
    _size.setHeight(height);
    if(collisionComponent())
    {
        collisionComponent()->setHeight(height);
    }
}

Vector2D Actor::baseVelocity() const
{
    return _baseVelocity;
}

void Actor::setBaseVelocity(const Vector2D &velocity)
{
    _baseVelocity = velocity;
}

int Actor::baseVelocityX() const
{
    return _baseVelocity.x();
}

int Actor::baseVelocityY() const
{
    return _baseVelocity.y();
}

void Actor::setBaseVelocityX(int velocityX)
{
    _baseVelocity.setX(velocityX);
}

void Actor::setBaseVelocityY(int velocityY)
{
    _baseVelocity.setY(velocityY);
}

int Actor::rotation() const
{
    if(_faceDirection == Vector2D(1, 0))
    {
        return 0;
    }
    else if(_faceDirection == Vector2D(-1, 0))
    {
        return 180;
    }
    else if(_faceDirection == Vector2D(0, 1))
    {
        return 90;
    }
    else if(_faceDirection == Vector2D(0, -1))
    {
        return -90;
    }
    else
    {
        throw "No this face direction";
    }
}

void Actor::setRotation(int rotation)
{
    int delta = qAbs<int>(this->rotation() - rotation);

    switch (rotation) {
    case 0:
        this->_faceDirection = {1, 0};
        break;
    case 180:
        this->_faceDirection = {-1, 0};
        break;
    case 90:
        this->_faceDirection = {0, 1};
        break;
    case -90:
        this->_faceDirection = {0, -1};
        break;
    default:
        throw "no this rotation";
        break;
    }

    if(delta == 90)
    {
        int temp = this->height();
        this->setHeight(this->width());
        this->setWidth(temp);
    }
}

bool Actor::isCollisionable() const
{
    return _isCollisionable;
}

void Actor::setIsCollisionable(bool isCollisionable)
{
    _isCollisionable = isCollisionable;
}

bool Actor::isMovable() const
{
    return _isMovable;
}

void Actor::setIsMovable(bool isMovable)
{
    _isMovable = isMovable;
}

SquareComponent *Actor::collisionComponent() const
{
    return _collisionComponent;
}

void Actor::setCollisionComponent(SquareComponent *collisionComponent)
{
    _collisionComponent = collisionComponent;
    _collisionComponent->setLocation(this->location());
    _collisionComponent->setSize(this->size());
}

bool Actor::isCollision(const Actor *otherActor) const
{
    if(!this->isCollisionable() || !otherActor->isCollisionable() ||
            this->_collisionComponent == nullptr || otherActor->collisionComponent() == nullptr)
        return false;

    return _collisionComponent->isCollision(otherActor->collisionComponent());
}

GameRunning *Actor::gameRunning() const
{
    return _gameRunning;
}

void Actor::setGameRunning(GameRunning *gameRunning)
{
    //  只可设置一次
    if(_gameRunning == nullptr && gameRunning != nullptr)
    {
        _gameRunning = gameRunning;
        _gameRunning->addActor(this, _isDynamic);
    }
}

Vector2D Actor::faceDirection() const
{
    return _faceDirection;
}

bool Actor::isDestroyed() const
{
    return _isDestroyed;
}

bool Actor::isDynamic() const
{
    return _isDynamic;
}

bool Actor::reachBoundary() const
{
    if(_gameRunning == nullptr)
        return false;

    QSize mapSize = _gameRunning->mapSize();
    if(location().x() < 0 || location().x() + width() > mapSize.width() ||
       location().y() < 0 || location().y() + height() > mapSize.height())
        return true;
    else
        return false;
}

void Actor::DestroyLater()
{
    if(!_isDestroyed && _gameRunning != nullptr)
    {
        _isDestroyed = true;
        _gameRunning->recycleActor(this);
    }
}

void Actor::in(QDataStream &inStream)
{
    inStream >> _location >> _size >> _faceDirection
             >> _baseVelocity >> _oldLocation;
}

void Actor::out(QDataStream &outStream) const
{
    outStream << _location << _size << _faceDirection
             << _baseVelocity << _oldLocation;
}

void Actor::tick()
{
    
}

void Actor::onHit(Actor*)
{

}
