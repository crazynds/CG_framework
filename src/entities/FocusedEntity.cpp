#include <entities/FocusedEntity.h>
#include <EngineState.h>

int FocusedEntity::tick(EngineState *state, double delta)
{
    focused = state->getFocusedEntity() == this;
    return TICK_OK;
}