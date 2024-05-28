#ifndef __Particle_H__
#define __Particle_H__

#include <list>
#include <stdio.h>
#include <vector>
#include <entities/Entity.h>
#include <entities/Ball.h>
#include <Color.h>

/**
 * Essa entidade é uma particula que vai viver na tela por um curto periodo de vida
 */

class Particle : public Entity
{
private:
    Vector2d moviment;
    Color color;

    double timeToLive;

public:
    Particle(Vector2d pos, Vector2d mov, Color color, double timeToLive) : Entity(pos), moviment(mov), color(color), timeToLive(timeToLive)
    {
    }

    virtual int tick(EngineState *state, double delta);
    virtual void render();

    virtual void entityName(char *buffer)
    {
        sprintf(buffer, "Particle");
    };
};

#endif