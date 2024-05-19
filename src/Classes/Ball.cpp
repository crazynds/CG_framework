#include <entities/Ball.h>
#include <entities/Map.h>
#include <entities/Screen.h>
#include <entities/Particle.h>
#include <Matrix3.h>
#include <gl_canvas2d.h>
#include <EngineState.h>
#include <time.h>

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

        particle -= delta;
        if (particle < 0)
        {
            particle += 0.05;
            Screen *s = (Screen *)state->getMainEntity();
            double vel = 3;
            for (int x = 0; x < rand() % 3 + 1; x++)
            {
                s->addEntity(new Particle(getPosition(), moviment * -0.1 + (Vector2d){rand() % 17 - 8.0, rand() % 17 - 8.0} * vel, Color::fromRGB(0xa0, 0xa0, 0xa0), 0.4));
            }
        }

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
