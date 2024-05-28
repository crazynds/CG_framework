#ifndef __BALL_H__
#define __BALL_H__

#include <list>
#include <stdio.h>
#include <vector>
#include <entities/Entity.h>
#include <Matrix3.h>

/**
 * Essa entidade é uma bolinha que vai se movimentar na tela e colidir com os blocos
 */

class Ball : public Entity
{
private:
    Matrix3Plan<double> transformation;

    double particle;

public:
    Vector2d moviment;
    bool moving = false;

    Ball();

    virtual int tick(EngineState *state, double delta);
    virtual void render();

    virtual void entityName(char *buffer)
    {
        sprintf(buffer, "Ball");
    };
};

#endif