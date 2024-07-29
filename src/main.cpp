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
 *    - Classes em C++ para definição de vetores
 *    - Sistema do controle do canhão
 *    - Sistema de movimentação do projétil.
 *    - Colisão do projétil com os alvos
 *    - Controle de FPS (controle real da velocidade da animação) (DE FORMA ESTÁTICA NO CÓDIGO, PODE SER ALTERADO NO DEFINE)
 *    - Explosões com partículas
 *    - Powerups
 *    - Formas de interação dos projéteis com os cantos dos quadrados.
 *    - Tela que se mexe de acordo com a posição. (Poderia dar uns 3 pontos né?)
 */

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <stdio.h>
#include <stdlib.h>
#include <chrono>

#include <EngineState.h>
#include <Scenes.h>
#include <Matrix3.h>
#include <windows.h>

#define MAX_FPS 60

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
   // Sleep(5);
}

void draw()
{
   // Clear screen
   globalState->render();
}

float tickNum = 0;
void render()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   tick();

   draw();

   glutSwapBuffers();
}

void end()
{
   delete globalState;
   exit(0);
}
void keyboard(unsigned char key, int, int)
{
   if (key == 27)
      end();
   globalState->keyboardHandler(key, true);
}
void keyboardUp(unsigned char key, int, int)
{
   globalState->keyboardHandler(key, false);
}
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   globalState->mouseHandlerAsync(button, state, wheel, direction, x, y);
}
void mouseClick(int button, int state, int x, int y)
{
   mouse(button, state, -2, -2, x, screenHeight - y);
}
void motion(int x, int y)
{
   mouse(-2, -2, -2, -2, x, screenHeight - y);
}
int main(void)
{
   int argc = 0;
   glutInit(&argc, NULL);

   // glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

   glutInitWindowSize(screenWidth, screenHeight);
   glutInitWindowPosition(450, 10);
   /* create window */
   glutCreateWindow("Trabalho 5: Mapa 3d");

   /* init */
   float abertura = 20.0;
   float znear = 1;
   float zfar = 23;
   float aspect = 1;
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_MAP2_VERTEX_3);

   globalState = new EngineState();
   globalState->setMainEntity(generateScene4());
   lastTime = std::chrono::system_clock::now();

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(abertura, aspect, znear, zfar);
   glMatrixMode(GL_MODELVIEW);

   glClearColor(0, 0, 0, 1);

   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   glEnable(GL_DEPTH_TEST);

   glutDisplayFunc(render);
   glutMouseFunc(mouseClick);
   glutPassiveMotionFunc(motion);
   glutMotionFunc(motion);
   glutKeyboardFunc(keyboard);
   glutKeyboardUpFunc(keyboardUp);
   glutIdleFunc(render);

   glutMainLoop();

   return 0;
}
