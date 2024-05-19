#ifndef __Canon_H__
#define __Canon_H__

#include <list>
#include <stdio.h>
#include <vector>
#include <entities/Entity.h>
#include <entities/Ball.h>
#include <Matrix3.h>

class Canon : public Entity
{
private:
    Matrix3Plan<double> transformationBody, transformationCanon;

    Ball ball;

    Vector2d mouse;
    double potency = 1;

public:
    Canon();

    virtual int tick(EngineState *state, double delta);
    virtual void render();
};

#endif