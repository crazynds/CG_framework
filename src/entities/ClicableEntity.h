#ifndef __CLICABLE_ENTITY_H__
#define __CLICABLE_ENTITY_H__

#include "../Classes/EngineState.h"
#include "Entity.h"
#include "../Classes/Vector2.h"

class ClicableEntity : public Entity
{
private:
    bool clicked = false, enter = false;
protected:
    Vector2d boxSize;
public:
    ClicableEntity(Vector2d _position, Vector2d _boxSize) : Entity(_position), boxSize(_boxSize)
    {
    }

    virtual void onMouseEnter(){}; // Not mandatory to be implemented
    virtual void onMouseExit(){};  // Not mandatory to be implemented
    virtual void onClick(){};      // Not mandatory to be implemented
    virtual void onRelease(){};    // Not mandatory to be implemented

    virtual int tick(EngineState *state, double delta)
    {
        /**
         * Click state handler
         */
        Vector2i mouse = state->getMousePosition();

        if (mouse.x == -1 && mouse.y == -1)
        {
            return TICK_OK;
        }
        if (state->getMouseState() == -3)
        {
            mouse.x = -1;
            mouse.y = -1;
        }

        if(mouse.x >= position.x && mouse.x <= position.x + boxSize.x && mouse.y >= position.y && mouse.y <= position.y + boxSize.y){
            if (!enter)
            {
                enter = true;
                onMouseEnter();
            }
            if(state->getMouseState() == 0 && state->getMouseButton() == 0){
                // clicked
                clicked = false;
                onClick();
                Vector2i mouse = state->getMousePosition();
                state->mouseHandler(-2, -3, -2, -2, mouse.x, mouse.y);
                return TICK_TO_FRONT;
            }
            state->mouseHandler(-2, -3, -2, -2, mouse.x, mouse.y);
        }else{
            if(enter){
                enter = false;
                onMouseExit();
            }
        }
        if (clicked && state->getMouseState() == 1 && state->getMouseButton() == 0)
        {
            onRelease();
        }
        return TICK_OK;
    };
    virtual void render() = 0;

    Vector2d getBoxSize(){
        return boxSize;
    }

    void setBoxSize(Vector2d _boxSize){
        boxSize = _boxSize;
    }
};

#endif