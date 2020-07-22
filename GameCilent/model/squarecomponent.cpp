﻿#include "squarecomponent.h"

SquareComponent::SquareComponent(QObject *parent) : QObject(parent)
{

}

SquareComponent::SquareComponent(int width, int height, QObject *parent) :
    QObject(parent), _width(width), _height(height)
{

}

int SquareComponent::y() const
{
    return _y;
}

void SquareComponent::setY(int y)
{
    _y = y;
}

int SquareComponent::x() const
{
    return _x;
}

void SquareComponent::setX(int x)
{
    _x = x;
}

int SquareComponent::height() const
{
    return _height;
}

void SquareComponent::setHeight(int height)
{
    _height = height;
}

bool SquareComponent::isCollision(const SquareComponent *otherComponent) const
{
    if(otherComponent == nullptr)
        return false;

    return this->x() < otherComponent->x() + otherComponent->width() &&
            this->x() + this->width() > otherComponent->x() &&
            this->y() < otherComponent->y() + otherComponent->height() &&
            this->y() + this->height() > otherComponent->y();
}

int SquareComponent::width() const
{
    return _width;
}

void SquareComponent::setWidth(int width)
{
    _width = width;
}
