#include <entities/Ball.h>
#include <entities/Map.h>
#include <Matrix3.h>
#include <gl_canvas2d.h>
#include <EngineState.h>

const extern double gravity;

Ball::Ball() : Entity({0, currentScreenHeight / 2.0}), moviment(0, 0)
{
}

const double radius = 6;
Map *map = nullptr;

int Ball::tick(EngineState *state, double delta)
{
    if (moving)
    {
        moviment += Vector2d(0, -gravity) * delta;

        map = (Map *)(state->getFocusedEntity());
        if (map != nullptr)
        {
            Vector2d mov = moviment * (delta + 0.01);
            float dif = mov.size();
            if (dif > radius)
            {
                dif = 1 / (delta + 0.01);
            }
            else
            {
                mov.normalize();
                mov *= radius;
                dif = 1 / (delta + 0.01) * (dif / radius);
            }
            moviment = map->checkColision(state, getPosition(), mov) * dif;
        }
        translate(moviment * delta);

        if (getPosition().y <= 100)
        {
            moving = false;
            return TICK_KILL_MYSELF;
        }
    }

    return TICK_OK;
}

void Ball::render()
{
    if (moving)
    {
        CV::color(WHITE);
        CV::circleFill(getPosition(), radius, 12);
        if (map != nullptr)
        {
            map->checkColision(nullptr, getPosition(), moviment, true);
        }
    }
}
