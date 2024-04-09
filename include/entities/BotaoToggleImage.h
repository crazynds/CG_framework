#ifndef __BOTAO_TOGGLE_IMAGE_H__
#define __BOTAO_TOGGLE_IMAGE_H__

/**
 * Essa entidade é mais especializada, e serve unicamente para quando for acionado, verificar se a entidade atualmente focada é uma imagem
 *  e se for, ele altera a exibição dela.
 */

#include <entities/Botao.h>
#include <entities/Image.h>

class BotaoToggleImage : public Botao
{
public:
    BotaoToggleImage(Vector2d pos, Vector2d box, const char *str) : Botao(pos, box, str)
    {
    }

    virtual int tick(EngineState *state, double delta)
    {
        if (isClicked())
            return Botao::tick(state, delta);

        Entity *e = state->getFocusedEntity();
        int s = Botao::tick(state, delta);

        if (isClicked() && e != nullptr)
        {
            char buffer[256];
            e->entityName(buffer);
            if (strcmp(buffer, "BMPImage") == 0)
            {
                BMPImage *img = (BMPImage *)e;
                img->toggle();
            }
            state->setFocusedEntity(e);
        }

        return s;
    }
};

#endif