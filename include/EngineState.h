#ifndef __ENGINE_STATE_H__
#define __ENGINE_STATE_H__

/**
 * Essa classe controla o estado da aplicação.
 * Basicamente ela faz o handler de os os inputs do usuario, separa qual entidade é o foco atual
 *  e armazena informações de teclado e mouse. Se as entidades precisarem de alguma informação
 *  da aplicação, é essa classe que elas vão consultar.
 */

#include "Vector2.h"
#include <stdio.h>
#include <list>
#include <GL/glut.h>

#define CONST_WINDOW_DIFF 80

extern int screenWidth, screenHeight;
extern int currentScreenWidth, currentScreenHeight;

class Entity;

struct MouseEvent
{
    int mouseX = 0, mouseY = 0;
    int mouseButton, mouseState, mouseWheel;
};

class EngineState
{
private:
    // Screen variables
    //    int windowX, windowY;
    // Mouse variables
    std::list<MouseEvent> mouseEvents;
    bool lock;

    int mouseX = 0,
        mouseY = 0;
    int lastReadMouseX = -1, lastReadMouseY = -1;
    int mouseButton, mouseState, mouseWheel;
    double avgDelta = 1;
    // Keyboard variables
    bool keyboard[256] = {false};

    // Geral
    unsigned long long int tickNumber = 0;
    unsigned long long int frame = 0;

    double sumTime = 0;
    double lastDebugTime = 0;

    bool clicking = false;

    Entity *mainEntity = 0;

    Entity *focusedEntity = 0;

    Vector2i windowPosition = {0, 0};

public:
    void render();

    void tick(double delta);

    void keyboardHandler(int key, bool state)
    {
        keyboard[key] = state;
    }

    void clearClick()
    {
        this->mouseHandler(-2, -3, -2, -2, mouseX, mouseY);
        clicking = false;
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
        if (x != -1 && y != -1)
        {
            mouseX = x;
            mouseY = y;
        }
        lastReadMouseX = x;
        lastReadMouseY = y;
    }
    void mouseHandlerAsync(int button, int state, int wheel, int direction, int x, int y)
    {
        if (state != -2 || mouseEvents.empty())
        {
            this->lock = true;
            MouseEvent event;
            event.mouseButton = button;
            event.mouseState = state;
            event.mouseWheel = direction;
            event.mouseX = x + windowPosition.x;
            event.mouseY = y + windowPosition.y - screenHeight - CONST_WINDOW_DIFF;
            mouseEvents.push_back(event);
            this->lock = false;
        }
    }

    Vector2i getScreenSize()
    {
        return {screenWidth, screenHeight};
    }
    Vector2i getWindowPosition()
    {
        // return {glutGet((GLenum)GLUT_WINDOW_X), currentScreenHeight - screenHeight - glutGet((GLenum)GLUT_WINDOW_Y)};
        return {windowPosition.x, windowPosition.y - screenHeight};
    }

    Vector2i getMousePosition()
    {
        return Vector2i(mouseX, mouseY);
    }
    Vector2i getExactLastReadedMousePosition()
    {
        return Vector2i(lastReadMouseX, lastReadMouseY);
    }

    int getMouseButton()
    {
        return mouseButton;
    }
    int getMouseWheel()
    {
        return mouseWheel;
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

    bool isDebugTick()
    {
        // Disabled
        return sumTime - lastDebugTime > 10 && false;
    }

    void setWindowFocus(Vector2i pos);
};

#endif