#ifndef FRONTEND_H_
#define FRONTEND_H_

#include <stdio.h>
#include <ncurses.h>
#include "brick_game.h"

#define WIDTH 10
#define HEIGHT 20

WINDOW* startFront();
void drawPointField(WINDOW *win, int **field);
void drawFigure(WINDOW *win, int figure[4][4], int x, int y);
void get_inputs();

#endif