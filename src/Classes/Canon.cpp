#include <entities/Canon.h>
#include <entities/Map.h>
#include <gl_canvas2d.h>
#include <EngineState.h>

Canon::Canon() : Entity({currentScreenWidth / 2.0, 200})
{
}

double rotation = 0;
double sizeCannon = 5;
const double speed = 600;
extern const double gravity = 240;

int Canon::tick(EngineState *state, double delta)
{

    rotation += 0.05;
    transformationBody.clear();
    transformationBody.addScaleMatrix(15, 15);
    transformationBody.addRotateMatrix(rotation);
    transformationBody.push();
    transformationBody.addTranslateMatrix(getPosition());

    mouse = state->getMousePosition();

    if (ball.moving)
    {
        state->setWindowFocus(Vector2i((int)ball.getPosition().x, (int)ball.getPosition().y));
    }
    else
    {
        state->setWindowFocus(Vector2i((int)getPosition().x, (int)getPosition().y));
        if (mouse.x != -1 && mouse.y != -1)
        {
            mouse -= getPosition(); // get relative diff
            double dist = mouse.normalize();
            potency = dist / 240;
            if (potency > 1.25)
                potency = 1.25;
            transformationCanon.clear();
            transformationCanon.addScaleMatrix(8, 8);
            transformationCanon.addRotateMatrix(atan2(mouse.y, mouse.x));
            transformationCanon.push();
            transformationCanon.addTranslateMatrix(getPosition());
        }
        if (state->isClicking())
        {
            Vector2d position = transformationCanon.compute({sizeCannon, 0});
            ball.moving = true;
            ball.moviment = mouse * speed * potency;
            ball.setPosition(position);
            state->clearClick();
        }
    }

    int ret = ball.tick(state, delta);
    if (ret == TICK_KILL_MYSELF)
    {
        position.x = ball.getPosition().x;
        Map *map = (Map *)state->getFocusedEntity();
        map->addLayer();
    }

    return TICK_OK;
}

void Canon::render()
{
    // Render ball before
    ball.render();

    CV::color(WHITE);

    Vector2d body[4] = {
        {-1, -1},
        {1, -1},
        {1, 1},
        {-1, 1}};

    for (Vector2d &vec : body)
    {
        vec = transformationBody.compute(vec);
    }
    CV::polygonFill(body, 4);

    CV::color(GREY);
    Vector2d cannon[4] = {
        {-1, -1},
        {-1, 1},
        {sizeCannon, 1},
        {sizeCannon, -1}};

    for (Vector2d &vec : cannon)
    {
        vec = transformationCanon.compute(vec);
    }
    CV::polygonFill(cannon, 4);
    if (!ball.moving)
    {
        CV::textf(getPosition() - Vector2d(80, 50), "Clique para atirar!");
        Vector2d position = transformationCanon.compute({sizeCannon, 0});
        CV::color(GREY);
        int rastroSize = 20;
        double timeSlice = 0.05;
        Vector2d speedVec = (mouse * speed * potency);

        for (int x = 1; x < rastroSize; x++)
        {
            position += speedVec * timeSlice;
            speedVec += Vector2d(0, -gravity) * timeSlice;
            CV::circleFill(position, (3 - (1.0 / rastroSize * x) * 2), 10);
        }
    }
}
