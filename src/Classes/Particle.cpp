#include <entities/Particle.h>
#include <gl_canvas2d.h>

const extern double gravity;

int Particle::tick(EngineState *state, double delta)
{
    timeToLive -= delta;
    if (timeToLive <= 0)
    {
        return TICK_KILL_MYSELF;
    }
    translate(moviment * delta);

    return TICK_OK;
}
void Particle::render()
{
    CV::color(color);
    CV::circleFill(getPosition(), 2, 3);
}