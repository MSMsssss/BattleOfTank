#ifndef SQUARECOMPONENT_H
#define SQUARECOMPONENT_H

#include <QObject>

class SquareComponent : public QObject
{
    Q_OBJECT
public:
    explicit SquareComponent(QObject *parent = nullptr);
    explicit SquareComponent(int width, int height, QObject *parent = nullptr);

protected:
    int _width = 0;
    int _height = 0;
    int _x = 0;
    int _y = 0;

public:
    int width() const;
    void setWidth(int width);

    int height() const;
    void setHeight(int height);

    bool isCollision(const SquareComponent* otherComponent) const;

    int x() const;
    void setX(int x);

    int y() const;
    void setY(int y);

    QPoint location() const;
    void setLocation(const QPoint& location);

    QSize size() const;
    void setSize(const QSize& size);

signals:

};

#endif // SQUARECOMPONENT_H
