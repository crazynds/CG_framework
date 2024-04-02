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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "gl_canvas2d.h"

int screenWidth = 1024, screenHeight = 820;

int opcao = 50;     
int mouseX, mouseY; 


void render()
{
   // Clear screen
   CV::color(WHITE);
   CV::rectFill(0,0, screenWidth, screenHeight);
   CV::translate(0, 0);


   CV::color(BLACK);
   CV::text(20, 500, "Programa Demo Canvas2D");

   char str[100];
   CV::textf(10, 300, "Mouse: (%d,%d)", mouseX, mouseY);
   CV::textf(10, 320, "Screen: (%d,%d)", screenWidth, screenHeight);

   Sleep(10); // nao eh controle de FPS. Somente um limitador de FPS.
}

void keyboard(int key)
{
   printf("\nTecla: %d", key);
}
void keyboardUp(int key)
{
   printf("\nLiberou: %d", key);
}
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   mouseX = x; // guarda as coordenadas do mouse para exibir dentro da render()
   mouseY = y;

   printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction, x, y);
}

int main(void)
{
   CV::init(screenWidth, screenHeight, "Titulo da Janela: Canvas 2D - Pressione 1, 2, 3");
   CV::run();
}
