#ifndef __FOCUSED_ENTITY_H__
#define __FOCUSED_ENTITY_H__

#include <entities/Entity.h>

class FocusedEntity : public Entity
{
private:
    bool focused = false;

public:
    FocusedEntity(Vector2d _position) : Entity(_position) {}

    virtual int tick(EngineState *state, double delta);

    bool isFocused()
    {
        return focused;
    }
};

#endif