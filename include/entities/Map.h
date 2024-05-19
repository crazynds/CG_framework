#ifndef __Map_H__
#define __Map_H__

#include <entities/Entity.h>
#include <EngineState.h>
#include <gl_canvas2d.h>

#define BLOCK_SIZE 40

/**
 * Essa entidade é mapa dos quadradinhos e que faz a verificação de colisão
 */

class Map : public Entity
{
private:
    int maxX, maxY;
    int **map;

public:
    Map();

    virtual int tick(EngineState *state, double delta)
    {
        state->setFocusedEntity(this);
        return TICK_OK;
    }

    virtual void render();

    virtual void entityName(char *buffer)
    {
        sprintf(buffer, "Map");
    };

    Vector2d checkColision(EngineState *state, Vector2d ballPos, Vector2d ballVelocity, bool draw = false);

    bool addLayer();
};

#endif