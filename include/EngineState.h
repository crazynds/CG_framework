#ifndef __ENGINE_STATE_H__
#define __ENGINE_STATE_H__

#include "Vector2.h"
#include <stdio.h>

extern int screenWidth, screenHeight;

class Entity;

class EngineState
{
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

    bool clicking = false;

    Entity *mainEntity = 0;

    Entity *focusedEntity = 0;

public:
    void render();

    void tick(double delta);

    void keyboardHandler(int key, bool state)
    {
        keyboard[key] = state;
    }
    void mouseHandler(int button, int state, int wheel, int direction, int x, int y)
    {
        mouseButton = button;
        mouseState = state;
        mouseWheel = wheel;
        if (state == 0)
            clicking = true;
        if (state == 1)
            clicking = false;
        mouseX = x;
        mouseY = y;
        lastReadMouseX = x;
        lastReadMouseY = y;
    }

    Vector2i getScreenSize()
    {
        return {screenWidth, screenHeight};
    }

    Vector2i getMousePosition()
    {
        return {mouseX, mouseY};
    }
    Vector2i getExactLastReadedMousePosition()
    {
        return {lastReadMouseX, lastReadMouseY};
    }

    int getMouseButton()
    {
        return mouseButton;
    }
    int getMouseState()
    {
        return mouseState;
    }
    void setFocusedEntity(Entity *entity)
    {
        this->focusedEntity = entity;
    }

    Entity *getFocusedEntity()
    {
        return this->focusedEntity;
    }

    void setMainEntity(Entity *entity);

    Entity *getMainEntity()
    {
        return this->mainEntity;
    }

    unsigned long long int getTickNumber()
    {
        return tickNumber;
    }

    unsigned long long int getFrame()
    {
        return frame;
    }

    bool isClicking()
    {
        return clicking;
    }

    bool getKeyboardState(int key)
    {
        if (key < 0 || key >= 256)
            return false;
        return keyboard[key];
    }
};

#endif