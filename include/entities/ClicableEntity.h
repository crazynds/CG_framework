#ifndef __CLICABLE_ENTITY_H__
#define __CLICABLE_ENTITY_H__

/**
 * Essa entidade se assemelha muito a entidade botão, porém ela é mais genérica, e permite que entidades que descendam dela definam
 *  a box de colisão.
 * Essa entidade também define a interface que possibilita que eventos sejam chamados como: onClick, onRelease, onMouseEnter e onMouseExit.
 */

#include <EngineState.h>
#include <entities/FocusedEntity.h>
#include <Vector2.h>

class ClicableEntity : public FocusedEntity
{
private:
    bool clicked = false, enter = false;

protected:
    Vector2d boxSize;

public:
    ClicableEntity(Vector2d _position, Vector2d _boxSize) : FocusedEntity(_position), boxSize(_boxSize)
    {
    }

    virtual void onMouseEnter(EngineState *state){}; // Not mandatory to be implemented
    virtual void onMouseExit(EngineState *state){};  // Not mandatory to be implemented
    virtual void onClick(EngineState *state){};      // Not mandatory to be implemented
    virtual void onRelease(EngineState *state){};    // Not mandatory to be implemented

    virtual bool checkColision(int x, int y)
    {
        return x >= position.x && x <= position.x + boxSize.x && y >= position.y && y <= position.y + boxSize.y;
    }

    virtual int tick(EngineState *state, double delta);
    virtual void render() = 0;

    Vector2d getBoxSize()
    {
        return boxSize;
    }

    void setBoxSize(Vector2d _boxSize)
    {
        boxSize = _boxSize;
    }

    bool isClicked()
    {
        return clicked;
    }
};

#endif