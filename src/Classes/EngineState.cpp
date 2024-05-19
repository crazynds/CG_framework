#include <EngineState.h>
#include <entities/Entity.h>
#include <gl_canvas2d.h>

int screenWidth = 500, screenHeight = 700;
int currentScreenWidth = 1080, currentScreenHeight = 1080;

void EngineState::render()
{
    frame += 1;
    CV::translate(-windowPosition.x, -windowPosition.y + CONST_WINDOW_DIFF + screenHeight);
    if (mainEntity)
        mainEntity->render();

    CV::textf(10, 10, "Time: %.1fs", sumTime);
    CV::translate(0, 0);
    CV::textf(screenWidth - 100, screenHeight - 20, "FPS: %d", (int)floor(1 / avgDelta));
}

void EngineState::tick(double delta)
{
    sumTime += delta;
    if (isDebugTick())
    {
        printf("Entidades sendo desenhadas: \n");
    }

    if (!mouseEvents.empty() && this->lock == false)
    {
        MouseEvent event = mouseEvents.front();
        mouseEvents.pop_front();
        this->mouseHandler(event.mouseButton, event.mouseState, event.mouseWheel, 0, event.mouseX, event.mouseY);
    }
    else
    {
        this->mouseHandler(-2, -3, -2, -2, -1, -1);
    }

    if (mainEntity)
        mainEntity->tick(this, delta);

    if (isDebugTick())
        lastDebugTime = sumTime;

    if (mouseState == 0)
    {
        setFocusedEntity(nullptr);
    }
    avgDelta = (avgDelta * 7 + delta) / 8;
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

void EngineState::setWindowFocus(Vector2i position)
{
    int posX = position.x - screenWidth / 2;
    if (currentScreenWidth - screenWidth < posX)
        posX = currentScreenWidth - screenWidth;
    if (posX < 0)
        posX = 0;

    int posY = position.y + screenHeight / 2 + CONST_WINDOW_DIFF;
    if (currentScreenHeight < posY)
        posY = currentScreenHeight;
    if (posY < screenHeight + CONST_WINDOW_DIFF)
        posY = screenHeight + CONST_WINDOW_DIFF;
    windowPosition = {(windowPosition.x * 2 + posX) / 3, (windowPosition.y * 2 + posY) / 3};
    CV::focusWindows(windowPosition);
    this->mouseHandler(-2, -3, -2, -2, -1, -1);
    mouseEvents.clear();
}