

#include <entities/Screen.h>
#include <entities/Botao.h>
#include <Position.h>
#include <entities/Canon.h>
#include <entities/Square.h>
#include <entities/Map.h>

Entity *generateScene2()
{
    Screen *screen = new Screen();

    screen->addEntity(new Canon());
    screen->addEntity(new Square({100, 100}, currentScreenWidth - 200, currentScreenHeight - 200));
    screen->addEntity(new Map());

    return screen;
}