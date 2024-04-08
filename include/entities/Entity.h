#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <Vector2.h>
#include <Position.h>
#include <stdio.h>

#define TICK_OK 0
#define TICK_TO_FRONT 1
#define TICK_TO_BACK 2
#define TICK_ERROR -1
#define TICK_KILL_MYSELF -2

class EngineState;

class Entity : public Position
{
public:
    Entity(Vector2d _position) : Position(_position)
    {
    }

    virtual ~Entity() {}

    virtual int tick(EngineState *state, double delta) = 0;
    virtual void render() = 0;

    virtual void entityName(char *buffer)
    {
        sprintf(buffer, "Entity");
    };
};

#endif