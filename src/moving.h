#ifndef MOVING_H_
#define MOVING_H_

#include "tetris.h"
#include "shifting.h"

void saveFigureDown();
void moveLeft();
void moveRight();
void moveDown();
int checkDownY();
void rotateFigure();
int tryAttachingOthers();
void copy_matrix(int a[4][4], int b[4][4]);



#endif