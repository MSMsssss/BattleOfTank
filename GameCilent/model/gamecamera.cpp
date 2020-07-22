#include "gamecamera.h"
#include "actor.h"
#include <QPainter>

GameCamera::GameCamera(const QSize& size) : _camera(size, QImage::Format_ARGB32)
{
    _camera.fill(QColor(255, 248, 220));   //  背景色
}

GameCamera::~GameCamera()
{

}

void GameCamera::showActor(const Actor &actor, const Vector2D &delta)
{
    QPoint relativeLocation = actor.oldLocation() + delta - this->location(); //Actor在相机中的相对位置
    QPainter painter(&_camera);
    painter.drawImage(relativeLocation, actor.printSource());
}

void GameCamera::showActor(const Actor& actor)
{
    QPoint relativeLocation = actor.location() - this->location(); //Actor在相机中的相对位置
    QPainter painter(&_camera);
    painter.drawImage(relativeLocation, actor.printSource());
}

QSize GameCamera::size() const
{
    return _camera.size();
}

int GameCamera::width() const
{
    return _camera.width();
}

int GameCamera::height() const
{
    return _camera.height();
}

QPoint GameCamera::location() const
{
    return _location;
}

void GameCamera::setLocation(const QPoint &location)
{
    _location = location;
    reset();
}

Vector2D GameCamera::velocity() const
{
    return _velocity;
}

void GameCamera::setVelocity(const Vector2D &velocity)
{
    _velocity = velocity;
}

void GameCamera::reset()
{
    _camera.fill(QColor(255, 248, 220));
}

QImage GameCamera::camera() const
{
    return _camera;
}

const QImage &GameCamera::cameraRef() const
{
    return _camera;
}
