/*********************************************************************
// Canvas para desenho, criada sobre a API OpenGL. Nao eh necessario conhecimentos de OpenGL para usar.
//  Autor: Cesar Tadeu Pozzer
//         05/2024
//
//  Pode ser utilizada para fazer desenhos, animacoes, e jogos simples.
//  Tem tratamento de mouse e teclado
//  Estude o OpenGL antes de tentar compreender o arquivo gl_canvas.cpp
//
//  Versao 2.0
//
//  Instru��es:
//	  Para alterar a animacao, digite numeros entre 1 e 3
// *********************************************************************/

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <stdio.h>
#include <stdlib.h>

#include "gl_canvas2d.h"
#include "Classes/EngineState.h"
#include "Scenes/Scenes.h"

unsigned long long int step = 0;

EngineState *globalState;


// void segueMouse()
// {
//    step += 1;
//    if (lastMouseX > 0 && lastMouseY > 0)
//    {
//       int xDiff = lastMouseX - screenWidth / 2;
//       lastMouseX = -1;
//       int yDiff = screenHeight / 2 - lastMouseY;
//       lastMouseY = -1;
//       printf("DIFF: %d %d\n", xDiff, yDiff);
//       windowX += xDiff / 3;
//       windowY += yDiff / 3;
//       windowX = std::min(windowX, 1920 - screenWidth);
//       if (windowX < 0)
//          windowX = 0;
//       windowY = std::min(windowY, 1024 - screenHeight);
//       if (windowY < 0)
//          windowY = 0;
//    }   
//    glutPositionWindow(windowX, windowY);
// }


void tick(){
   globalState->tick(0.015);
   // 15 ms
}

void draw(){
   // Clear screen
   CV::color(WHITE);
   CV::rectFill({0,0}, globalState->getScreenSize());
   CV::translate(0, 0);

   CV::color(BLACK);
   CV::text(20, 500, "Programa Demo Canvas2D");

   CV::textf(10, 300, "Mouse: (%d,%d)", globalState->getMousePosition().x, globalState->getMousePosition().y);
   CV::textf(10, 320, "Screen: (%d,%d)", screenWidth, screenHeight);
   globalState->render();
}


void render()
{
   // Run multiple tickes
   tick();

   draw();

   Sleep(10); // nao eh controle de FPS. Somente um limitador de FPS.
}

void end(){
   delete globalState;
   exit(0);
}
void keyboard(int key)
{
   if (key == 27)
      end();
   globalState->keyboardHandler(key, true);
}
void keyboardUp(int key)
{
   globalState->keyboardHandler(key, false);
}
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   globalState->mouseHandler(button, state, wheel, direction, x, y);
}
int main(void)
{
   globalState = new EngineState();
   CV::init(screenWidth, screenHeight, "Titulo da Janela: Canvas 2D - Pressione 1, 2, 3");
   globalState->setMainEntity(generateScene1());
   CV::run();
}
