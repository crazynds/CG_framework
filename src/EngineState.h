#ifndef __ENGINE_STATE_H__
#define __ENGINE_STATE_H__


#include "Vector2.h"

int screenWidth = 1200, screenHeight = 800;

class EngineState{
private:
    // Screen variables
//    int windowX, windowY;
    // Mouse variables
    int mouseX = 0, mouseY = 0;
    int lastReadMouseX = -1, lastReadMouseY = -1;
    int mouseButton, mouseState, mouseWheel;
    // Keyboard variables
    bool keyboard[256] = {false};

    // Geral
    unsigned long long int tickNumber = 0;
    unsigned long long int frame = 0;

public:
    void render(){
        frame += 1;
    }

    void tick(){
        lastReadMouseX = -1;
        lastReadMouseY = -1;
        tickNumber += 0;
    }

    void keyboardHandler(int key, bool state){
        keyboard[key] = state;
    }
    void mouseHandler(int button, int state, int wheel, int direction, int x, int y){
        mouseButton = button;
        mouseState = state;
        mouseWheel = wheel;
        mouseX = x;
        mouseY = y;
        lastReadMouseX = x;
        lastReadMouseY = y;
    }


    Vector2i getScreenSize(){
        return {screenWidth,screenHeight};
    }

    Vector2i getMousePosition(){
        return {mouseX,mouseY};
    }
    Vector2i getExactLastReadedMousePosition(){
        return {lastReadMouseX,lastReadMouseY};
    }

    int getMouseButton(){
        return mouseButton;
    }
    int getMouseState(){
        return mouseState;
    }

    unsigned long long int getTickNumber(){
        return tickNumber;
    }

    unsigned long long int getFrame(){
        return frame;
    }

    bool getKeyboardState(int key){
        if(key <0 || key >= 256) return false;
        return keyboard[key];
    }
};



#endif