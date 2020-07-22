#ifndef ACTOR_H
#define ACTOR_H

#include <QImage>
using Vector2D = QPoint;
class SquareComponent;

class Actor
{
public:
    explicit Actor(bool isDynamic = true);

    friend QDataStream& operator>>(QDataStream& in, Actor& actor);
    friend QDataStream& operator<<(QDataStream& out, const Actor& actor);

    QPoint oldLocation() const;
    void setOldLocation(const QPoint &oldLocation);

    QPoint location() const;
    Vector2D delta() const;
    void setLocation(const QPoint &location);
    int x() const;
    int y() const;
    void setX(int x);
    void setY(int y);

    QSize size() const;
    void setSize(const QSize &size);
    int width() const;
    int height() const;
    void setWidth(int width);
    void setHeight(int height);

    Vector2D baseVelocity() const;
    void setBaseVelocity(const Vector2D &baseVelocity);
    int baseVelocityX() const;
    int baseVelocityY() const;
    void setBaseVelocityX(int baseVelocityX);
    void setBaseVelocityY(int baseVelocityY);

    int rotation() const;
    void setRotation(int rotation);

    bool isCollisionable() const;
    void setIsCollisionable(bool isCollisionable);

    bool isMovable() const;
    void setIsMovable(bool isMovable);

    QImage *source() const;
    void setSource(QImage *source);
    virtual QImage printSource() const;

    Vector2D faceDirection() const;
    void setFaceDirection(const Vector2D &faceDirection);

    bool isDestroyed() const;

    bool isDynamic() const;

    virtual void in(QDataStream& inStream);
    virtual void out(QDataStream& outStream) const;

protected:
    /*  位置  */
    QPoint _oldLocation = {0, 0};
    QPoint _location = {0, 0};

    /*  尺寸  */
    QSize _size = {0, 0};

    /*  速度  */
    Vector2D _baseVelocity = {0, 0};

    Vector2D _faceDirection = {1, 0};   //朝向四个方向

    bool _isCollisionable = true;   //可碰撞的
    bool _isMovable = true; //可移动的
    bool _isDestroyed = false;  //已被销毁
    bool _isDynamic = true;
    QImage *_source; //资源
};

#endif // ACTOR_H
