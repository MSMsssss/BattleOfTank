#ifndef GAMECAMERA_H
#define GAMECAMERA_H
#include <QPoint>
#include <QSize>
#include <QImage>

class Actor;
using Vector2D = QPoint;

class GameCamera
{
public:
    GameCamera() = default;
    GameCamera(const QSize& size);
    virtual ~GameCamera();
    void showActor(const Actor& actor);   //在相机中显示Actor
    void showActor(const Actor& actor, const Vector2D& delta);

    QSize size() const;
    int width() const;
    int height() const;

    QPoint location() const;
    //  移动位置将重置相机画面
    void setLocation(const QPoint &location);

    Vector2D velocity() const;
    void setVelocity(const Vector2D &velocity);

    //  重置屏幕内容
    void reset();

    QImage camera() const;
    const QImage& cameraRef() const;

protected:
    QImage _camera;    //相机内容
    QPoint _location = {0, 0};   //相机在地图中的位置
    Vector2D _velocity = {0, 0};    //  相机的移动速度
};

#endif // GAMECAMERA_H
