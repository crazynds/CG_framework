#ifndef __PIN_H__
#define __PIN_H__

#include <entities/ClicableEntity.h>
#include <Color.h>
#include <gl_canvas2d.h>

class Pin : public ClicableEntity
{
private:
    bool hover = 0;
    Color normal, hoverColor, clickColor;

public:
    Pin(Vector2d pos) : ClicableEntity(pos, {10, 10})
    {
        normal = Color::fromRGB(0xc0, 0xc0, 0xc0);
        hoverColor = Color::fromRGB(0xA0, 0xa0, 0xa0);
        clickColor = Color::fromRGB(0x10, 0xf0, 0x10);
    }

    virtual void onMouseEnter()
    {
        hover = true;
    };
    virtual void onMouseExit()
    {
        hover = false;
    };
    virtual bool checkColision(int x, int y)
    {
        float dx = (x - position.x);
        float dy = (y - position.y);
        return dx * dx + dy * dy < boxSize.x * boxSize.x;
    }

    virtual int tick(EngineState *state, double delta)
    {
        int status = ClicableEntity::tick(state, delta);
        if (isClicked())
        {
            Vector2i mouse = state->getMousePosition();
            setPosition(mouse);
        }
        return status;
    }

    virtual void render()
    {
        CV::color(isClicked() ? clickColor : (hover ? hoverColor : normal));
        CV::circleFill(position.x, position.y, boxSize.x, 12);
        CV::color(BLACK);
        CV::circle(position.x, position.y, boxSize.x, 20);
    }

    void setColor(Color color)
    {
        normal = color;
    }

    void setHoverColor(Color color)
    {
        hoverColor = color;
    }

    void setClickColor(Color color)
    {
        clickColor = color;
    }
};

#endif