#include "blockactor.h"
#include "model/tankactor.h"
#include "model/blockactor.h"
#include "model/projectileactor.h"
#include "helpfultool.h"

BlockActor::BlockActor(bool isDynamic) :
    Actor(isDynamic)
{

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

QImage BlockActor::printSource() const
{
    QMatrix matrix;
    matrix.rotate(rotation());

    QImage temp = _source->scaled(_size).transformed(QTransform(matrix));
    int alpha = qMax<int>(255 * this->health() / BlockActor::InitialHealth, 50);
    HelpfulTool::setAlpha(temp, alpha);

    return temp;
}
