#ifndef __FOCUSED_ENTITY_H__
#define __FOCUSED_ENTITY_H__

/**
 * Essa entidade ela se especializa em verificar se ela esta sendo focada ou não.
 * É bem importante para a própria entidade saber se ela tem o foco do usuario e desenhar informações que podem ser uteis para o usuário.
 */

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