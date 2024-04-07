#ifndef __POSITION_H__
#define __POSITION_H__

#include "Vector2.h"

extern int screenWidth, screenHeight;

class Position
{
protected:
    Vector2d position;

public:
    Position(Vector2d _position) : position(_position) {}

    static Position from(Position _position)
    {
        return Position(_position.getPosition());
    }
    static Position from(Position *_position)
    {
        return Position(_position->getPosition());
    }

    static Position from(Vector2d _position)
    {
        return Position(_position);
    }

    Vector2d getPosition()
    {
        return position;
    }

    void translate(Vector2d vec)
    {
        position += vec;
    }

    void setPosition(Vector2d _position)
    {
        position = _position;
    }

    Position toLeft(double val)
    {
        this->translate({-val, 0});
        return *this;
    }

    Position toRight(double val)
    {
        this->translate({val, 0});
        return *this;
    }

    Position toTop(double val)
    {
        this->translate({0, val});
        return *this;
    }

    Position toBottom(double val)
    {
        this->translate({0, -val});
        return *this;
    }
};

const Vector2d TOP_LEFT = Vector2d(0, screenHeight);
const Vector2d TOP_RIGHT = Vector2d(screenWidth, screenHeight);
const Vector2d BOTTOM_LEFT = Vector2d(0, 0);
const Vector2d BOTTOM_RIGHT = Vector2d(screenWidth, 0);

const Vector2d CENTER = Vector2d(screenWidth / 2, screenHeight / 2);
const Vector2d TOP_CENTER = Vector2d(screenWidth / 2, screenHeight);
const Vector2d BOTTOM_CENTER = Vector2d(screenWidth / 2, 0);

#endif