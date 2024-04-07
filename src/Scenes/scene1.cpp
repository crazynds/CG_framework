

#include "../Classes/Screen.h"
#include "../Classes/Botao.h"
#include "../Classes/Position.h"

Screen *paintScreen;

Entity *generateScene1()
{
    Screen *screen = new Screen();
    Botao *b1 = new Botao(
        Position::from(TOP_LEFT)
            .toRight(100)
            .toBottom(60)
            .getPosition(),
        {100, 40}, "a.bpm");
    Botao *b2 = new Botao(
        Position::from(b1)
            .toRight(120)
            .getPosition(),
        {100, 40}, "b.bpm");
    Botao *b3 = new Botao(
        Position::from(b2)
            .toRight(120)
            .getPosition(),
        {100, 40}, "c.bpm");

    paintScreen = new Screen()

    screen->addEntity(b1);
    screen->addEntity(b2);
    screen->addEntity(b3);
    screen->addEntity(paintScreen);
    return screen;
}
