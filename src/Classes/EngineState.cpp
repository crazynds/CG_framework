#include <EngineState.h>
#include <entities/Entity.h>
#include <gl_canvas2d.h>

int screenWidth = 1200, screenHeight = 800;

void EngineState::render()
{
    frame += 1;
    if (mainEntity)
        mainEntity->render();

    CV::textf(10, 10, "Time: %.1fs", sumTime);
}

void EngineState::tick(double delta)
{
    sumTime += delta;
    if (isDebugTick())
    {
        printf("Entidades sendo desenhadas: \n");
    }

    if (mainEntity)
        mainEntity->tick(this, delta);

    if (isDebugTick())
        lastDebugTime = sumTime;

    if (mouseState == 0)
        setFocusedEntity(nullptr);
    lastReadMouseX = -1;
    lastReadMouseY = -1;
    mouseState = -2;
    tickNumber += 1;
}

void EngineState::setMainEntity(Entity *entity)
{
    if (this->mainEntity != 0)
        delete this->mainEntity;
    this->mainEntity = entity;
}