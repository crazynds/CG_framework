#ifndef __Engine_H__
#define __Engine_H__

#include <list>
#include <stdio.h>
#include <vector>
#include <entities/Entity.h>
#include <entities/Ball.h>
#include <Matrix4.h>

/**
 * Essa entidade é o canhão que faz o disparo da bolinha
 */

class Engine : public Entity
{
private:
    double rotation = 0;
    double cameraRotation = 0;
    double speedEngine = 3;

    double zoom = 1;
    int pistoes = 1;
    int exibe = 0;

    bool only2d = false;
    bool onlyLines = true;
    bool perspective = true;

public:
    Engine();

    virtual int tick(EngineState *state, double delta);
    virtual void render();

    virtual void entityName(char *buffer)
    {
        sprintf(buffer, "Engine");
    };
};

#endif