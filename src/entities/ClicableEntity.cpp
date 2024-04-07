#include <entities/ClicableEntity.h>

int ClicableEntity::tick(EngineState *state, double delta)
{
    FocusedEntity::tick(state, delta);

    /**
     * Click state handler
     */
    Vector2i mouse = state->getMousePosition();

    if (mouse.x == -1 && mouse.y == -1)
    {
        return TICK_OK;
    }

    if (clicked && !state->isClicking())
    {
        onRelease();
        clicked = false;
    }

    if (state->getMouseState() == -3)
    {
        mouse.x = -1;
        mouse.y = -1;
    }

    if (checkColision(mouse.x, mouse.y))
    {
        if (!enter)
        {
            enter = true;
            onMouseEnter();
        }
        if (state->getMouseState() == 0 && state->getMouseButton() == 0)
        {
            // clicked
            clicked = true;
            state->setFocusedEntity(this);
            onClick();
            Vector2i mouse = state->getMousePosition();
            state->mouseHandler(-2, -3, -2, -2, mouse.x, mouse.y);
            return TICK_TO_FRONT;
        }
        state->mouseHandler(-2, -3, -2, -2, mouse.x, mouse.y);
    }
    else
    {
        if (enter)
        {
            enter = false;
            onMouseExit();
        }
    }
    return TICK_OK;
};