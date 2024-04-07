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



    virtual int tick(EngineState *state, double delta){
        std::vector<Entity*> front;
        std::vector<Entity*> end;
        for(auto i = entities.begin(); i != entities.end(); i++){
            int result = (*i)->tick(state, delta);
            switch(result){
                case TICK_TO_BACK:
                    end.push_back(*i);
                    entities.erase(i);
                    break;
                case TICK_TO_FRONT:
                    front.push_back(*i);
                    entities.erase(i);
                    break;
                case TICK_KILL_MYSELF:
                    delete *i;
                    entities.erase(i);
                    break;
                case TICK_OK:
                default:
                    continue;
            }
        }
        entities.insert(entities.end(), end.begin(), end.end());    // append to the end
        entities.insert(entities.begin(), front.begin(), front.end()); // append to the front
        return TICK_OK;
    }
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