#ifndef __SCREEN_H__
#define __SCREEN_H__


#include <list>
#include <stdio.h>
#include <vector>
#include "../entities/Entity.h"

class Screen: 
public Entity{
private:
    std::list<Entity*> entities;

public:
    Screen():
        Entity({0,0})
    {

    }

    ~Screen(){
        for(auto i = entities.begin(); i != entities.end(); i++){
            delete *i;
        }
    }

    virtual int tick(EngineState *state, double delta);
    virtual void render(){
        for(auto i = entities.rbegin(); i != entities.rend(); i++){
            (*i)->render();
        }
    }


    void addEntity(Entity* ptr){
        entities.push_back(ptr);
    }

};


#endif