#include "model/actor.h"
#include "model/squarecomponent.h"
#include <QPoint>
#include <QMatrix>

Actor::Actor(bool isDynamic)
{
    _isDynamic = isDynamic;
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

Vector2D Actor::delta() const
{
    return location() - oldLocation();
}

void Actor::setLocation(const QPoint &location)
{
    _location = location;
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
}

void Actor::setY(int y)
{
    _location.setY(y);
}

QSize Actor::size() const
{
    return _size;
}

void Actor::setSize(const QSize &size)
{
    _size = size;
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
}

void Actor::setHeight(int height)
{
    _size.setHeight(height);
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

QImage Actor::printSource() const
{
    QMatrix matrix;
    matrix.rotate(rotation());

    return _source->transformed(QTransform(matrix)).scaled(this->size());
}

Vector2D Actor::faceDirection() const
{
    return _faceDirection;
}

void Actor::setFaceDirection(const Vector2D &faceDirection)
{
    _faceDirection = faceDirection;
}

bool Actor::isDestroyed() const
{
    return _isDestroyed;
}

bool Actor::isDynamic() const
{
    return _isDynamic;
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

QImage *Actor::source() const
{
    return _source;
}

void Actor::setSource(QImage *source)
{
    _source = source;
}
