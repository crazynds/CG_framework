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
#include <Matrix3.h>
#include <windows.h>

#define MAX_FPS 35

unsigned long long int step = 0;
std::chrono::time_point<std::chrono::system_clock> lastTime;

EngineState *globalState;

void tick()
{
   const double MIN_DELTA = 1000.0 / MAX_FPS;
   auto now = std::chrono::system_clock::now();
   double delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTime).count();
   if (delta < MIN_DELTA)
   {
      Sleep(MIN_DELTA - delta);
      now = std::chrono::system_clock::now();
      delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTime).count();
   }
   lastTime = now;
   delta /= 1000.0;
   globalState->tick(delta);

   // Even with max fps, delay some time to update screen position;
   Sleep(5);
}

void draw()
{
   // Clear screen
   CV::clear(BLACK.r, BLACK.g, BLACK.b);
   CV::translate(0, 0);

   //   CV::textf(10, 300, "Mouse: (%d,%d)", globalState->getMousePosition().x, globalState->getMousePosition().y);
   //   CV::textf(10, 320, "Screen: (%d,%d)", screenWidth, screenHeight);
   globalState->render();
}

void render()
{
   tick();

   draw();
}

void end()
{
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
   CV::init(screenWidth, screenHeight, "Trabalho 2: Quadrados secretos");
   globalState->setMainEntity(generateScene2());
   lastTime = std::chrono::system_clock::now();
   CV::run();
}
