#ifndef __CLICABLE_ENTITY_H__
#define __CLICABLE_ENTITY_H__

#include "Entity.h"
#include "../Vector2.h"

class ClicableEntity:Entity{
private:
    bool clicked = false, enter = false;
protected:
    Vector2d boxSize;
public:

    ClicableEntity(Vector2d _position, Vector2d _boxSize):
        Entity(_position){
        boxSize = _boxSize;
    }

    virtual void onMouseEnter()=0;   // Not mandatory to be implemented
    virtual void onMouseExit()=0;   // Not mandatory to be implemented
    virtual void onClick()=0;   // Not mandatory to be implemented
    virtual void onRelease()=0; // Not mandatory to be implemented

    virtual void tick(const EngineState *state, double delta){
        /**
         * Click state handler
         */
        Vector2i mouse = state->getExactLastReadedMousePosition();

        if(mouse.x >= position.x && mouse.x <= position.x + boxSize.x && mouse.y >= position.y && mouse.y <= position.y + boxSize.y){
            if(!enter){
                enter = true;
                if(onMouseEnter!=0)
                    onMouseEnter();
            }
            if(state->getMouseState() == 0 && state->getMouseButton() == 0){
                // clicked
                clicked = false;
                if(onClick!=0)
                    onClick();
            }
        }else{
            if(enter){
                enter = false;
                if(onMouseExit!=0)
                    onMouseExit();
            }
        }
        if(clicked && state->getMouseState() == 1 && state->getMouseButton() == 0){
            if(onRelease!=0)
                onRelease();
        }
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