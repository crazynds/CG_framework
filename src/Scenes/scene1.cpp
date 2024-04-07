

#include <entities/Screen.h>
#include <entities/Botao.h>
#include <Position.h>
#include <entities/Image.h>
#include <vector>
#include <string>

Screen *paintScreen;

void addImg(void *arg)
{
    char *args = (char *)arg;
    char buffer[256] = "./images/";
    strcat(buffer, args);
    BMPImage *a = BMPImage::readBMP(buffer);
    if (a != nullptr)
    {
        paintScreen->addEntity(a);
    }
}

char *imgs[] = {
    "a.bmp",
    "b.bmp",
    "c.bmp",
};

Entity *generateScene1()
{
    Screen *screen = new Screen();

    Botao *last = nullptr;
    for (int x = 0; x < 3; x++)
    {
        Vector2d pos;
        if (last == nullptr)
        {
            pos = Position::from(TOP_LEFT)
                      .toRight(100)
                      .toBottom(60)
                      .getPosition();
        }
        else
        {
            pos = Position::from(last)
                      .toRight(120)
                      .getPosition();
        }
        Botao *b1 = new Botao(pos, {100, 40}, imgs[x]);
        b1->addEventListenerOnClick(addImg, (void *)imgs[x]);
        screen->addEntity(b1);
        last = b1;
    }
    paintScreen = new Screen();

    screen->addEntity(paintScreen);
    return screen;
}
