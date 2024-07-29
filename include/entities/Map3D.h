#ifndef __Map3d_H__
#define __Map3d_H__

#include <entities/Entity.h>
#include <EngineState.h>
#include <Vector3.h>

struct chunk
{
    GLfloat ctrlPoints[4][4][3];
};

/**
 * Essa entidade é mapa 3d
 */

class Map3D : public Entity
{
private:
    struct chunk **map;

    double angle, angleInclination;
    double speed;

public:
    Map3D();

    virtual int tick(EngineState *state, double delta);

    virtual void render();

    virtual void entityName(char *buffer)
    {
        sprintf(buffer, "Map3D");
    };
};

#endif