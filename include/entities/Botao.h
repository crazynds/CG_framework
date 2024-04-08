#ifndef __BOTAO_H__
#define __BOTAO_H__

#include <Color.h>
#include <gl_canvas2d.h>
#include <entities/ClicableEntity.h>

typedef void (*EventFunction)(void *arg);

class Botao : public ClicableEntity
{
  char label[100];
  int len = 0;
  bool hover = false;
  EventFunction clickFunction = (EventFunction)0;

  void *arg;

public:
  Botao(Vector2d pos, Vector2d boxSize, const char *_label) : ClicableEntity(pos, boxSize)
  {
    pos += boxSize * -0.5;
    strcpy(label, _label);
    len = strlen(label);
  }

  void addEventListenerOnClick(EventFunction clickFunction, void *arg)
  {
    this->clickFunction = clickFunction;
    this->arg = arg;
  }

  virtual void render()
  {
    if (!hover)
      CV::color(Color::fromRGB(0xa8, 0xa9, 0xaa));
    else
      CV::color(Color::fromRGB(0xe8, 0xe9, 0xea));
    CV::rectFill(this->position.x - 2, this->position.y - 2, this->position.x + this->boxSize.x + 2, this->position.y + this->boxSize.y + 2);

    CV::color(Color::fromRGB(0xe8, 0xe9, 0xea));
    CV::rectFill(this->position.x, this->position.y, this->position.x + this->boxSize.x, this->position.y + this->boxSize.y);

    CV::color(BLACK);
    CV::text(this->position.x + this->boxSize.x / 2 - len * 5, this->position.y + this->boxSize.y / 2 - 3, label); // escreve o label do botao mais ou menos ao centro.
  }

  virtual void onMouseEnter()
  {
    hover = true;
  }

  virtual void onMouseExit()
  {
    hover = false;
  }

  virtual void onClick()
  {
    if (clickFunction != 0)
      clickFunction(arg);
  }

  virtual void entityName(char *buffer)
  {
    sprintf(buffer, "Button (%s)", label);
  };
};

#endif
