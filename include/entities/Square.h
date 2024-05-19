#ifndef __Square_H__
#define __Square_H__

#include <entities/Entity.h>
#include <gl_canvas2d.h>

/**
 * Entidade que representa um quadrado na tela.
 */

class Square : public Entity
{
private:
    int sizeX, sizeY;

public:
    Square(Vector2d position, int sizeX, int sizeY) : Entity(position), sizeX(sizeX), sizeY(sizeY)
    {
    }

    virtual int tick(EngineState *state, double delta) { return TICK_OK; };
    virtual void render()
    {

        CV::color(BLUE);
        CV::rect(getPosition().x, getPosition().y, getPosition().x + sizeX, getPosition().y + sizeY);

        CV::color(RED);
        CV::line({100, 120}, {currentScreenWidth - 100.0, 120});
    }
};

#endif