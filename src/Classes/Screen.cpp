#include <entities/Screen.h>
#include <EngineState.h>

int Screen::tick(EngineState *state, double delta)
{
    std::vector<Entity *> front;
    std::vector<Entity *> end;
    for (auto i = entities.begin(); i != entities.end(); i++)
    {
        int result = (*i)->tick(state, delta);
        switch (result)
        {
        case TICK_TO_BACK:
            end.push_back(*i);
            i = entities.erase(i);
            break;
        case TICK_TO_FRONT:
            front.push_back(*i);
            i = entities.erase(i);
            break;
        case TICK_KILL_MYSELF:
            delete *i;
            i = entities.erase(i);
            break;
        case TICK_OK:
        default:
            continue;
        }
    }
    entities.insert(entities.end(), end.begin(), end.end());       // append to the end
    entities.insert(entities.begin(), front.begin(), front.end()); // append to the front

    if (state->isDebugTick())
    {
        char buffer[256];
        for (auto i = entities.begin(); i != entities.end(); i++)
        {
            (*i)->entityName(buffer);
            printf("[DEBUG]%s\n", buffer);
        }
    }
    return TICK_OK;
}