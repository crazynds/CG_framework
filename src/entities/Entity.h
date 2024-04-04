#ifndef __ENTITY_H__
#define __ENTITY_H__


#include "../EngineState.h"

class Entity{
protected:
    Vector2d position;
public:

    Entity(Vector2d _position){
        position = _position;
    }

    virtual void tick(const EngineState *state, double delta) = 0;
    virtual void render() = 0;


    Vector2d getPosition(){
        return position;
    }

    void translate(Vector2d vec){
        position += vec;
    }

    void setPosition(Vector2d _position){
        position = _position;
    }

};

#endif