#ifndef __BALL_H__
#define __BALL_H__

#include <list>
#include <stdio.h>
#include <vector>
#include <entities/Entity.h>
#include <Matrix3.h>

class Ball : public Entity
{
private:
    Matrix3Plan<double> transformation;

public:
    Vector2d moviment;
    bool moving = false;

    Ball();

    virtual int tick(EngineState *state, double delta);
    virtual void render();
};

#endif