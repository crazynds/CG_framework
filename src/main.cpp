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

/**
 * O que foi implementado:
 *    -Carregar imagens colorida (RGB) em formato BMP. Utilize imagens retangulares pequenas
 * (entre 200 e 300 pixels) para facilitar a exibição dentro da canvas2D.
 *    -Mover cada imagem individualmente dentro da canvas2D com uso do mouse. Deve-se ter o
 * conceito de imagem corrente selecionada para que sejam aplicados processamentos sobre
 * ela. Pode-se indicar a imagem selecionada com uma moldura. A imagem selecionada deve
 * aparecer na frente das outras.
 *    -Reexibir a imagem selecionada, nos canais RGB e Luminancia.
 *    -Rotacionar a imagem no eixo vertical e horizontal.
 *    -Plotar o histograma de cada canal RGB e da luminância.
 *
 * Bonus implementado:
 *    -Aplicar uma rotação qualquer a imagem. A rotação deve ser feita com o uso do mouse.
 *
 */

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <stdio.h>
#include <stdlib.h>
#include <chrono>

#include <gl_canvas2d.h>
#include <EngineState.h>
#include <Scenes.h>

unsigned long long int step = 0;
std::chrono::time_point<std::chrono::system_clock> lastTime;

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
   auto now = std::chrono::system_clock::now();
   double delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTime).count();
   delta /= 1000.0;
   lastTime = now;
   globalState->tick(delta);
}

void draw(){
   // Clear screen
   CV::clear(WHITE.r, WHITE.g, WHITE.b);
   CV::translate(0, 0);

   CV::color(BLACK);
   CV::text(20, 500, "Programa Demo Canvas2D");

   CV::textf(10, 300, "Mouse: (%d,%d)", globalState->getMousePosition().x, globalState->getMousePosition().y);
   CV::textf(10, 320, "Screen: (%d,%d)", screenWidth, screenHeight);
   globalState->render();
}


void render()
{
   tick();

   draw();
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
   globalState->mouseHandlerAsync(button, state, wheel, direction, x, y);
}
int main(void)
{
   globalState = new EngineState();
   CV::init(screenWidth, screenHeight, "Trabalho 1");
   globalState->setMainEntity(generateScene1());
   lastTime = std::chrono::system_clock::now();
   CV::run();
}
