#include "EngineState.h"
#include "../entities/Entity.h"

int screenWidth = 1200, screenHeight = 800;

void EngineState::render()
{
    frame += 1;
    if (mainEntity)
        mainEntity->render();
}

void EngineState::tick(double delta)
{
    if (mainEntity)
        mainEntity->tick(this, delta);
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