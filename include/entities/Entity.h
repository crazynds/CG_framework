#ifndef __ENTITY_H__
#define __ENTITY_H__

/**
 * Essa classa defini a menor unidade que vai gerenciada na tela.
 * Ela implementa interfaces importantes para que seja processada e renderizada, e ele é a base para todas as outras classes.
 */

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