

#include <entities/Screen.h>
#include <entities/Botao.h>
#include <entities/BotaoToggleImage.h>
#include <Position.h>
#include <entities/Image.h>
#include <vector>
#include <string>
#include <entities/BadApple.h>

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

const char *imgs[] = {
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

    Botao *b1 = new BotaoToggleImage(Position::from(last)
                                         .toRight(120)
                                         .getPosition(),
                                     {160, 40}, "Altera Imagem");
    screen->addEntity(b1);

    last = b1;

    paintScreen = new Screen();

    paintScreen->addEntity(new BadApple());
    screen->addEntity(paintScreen);

    return screen;
}

