#ifndef TETRIS_H_
#define TETRIS_H_

#include "brick_game.h"
#include "figure.h"
#include "frontend.h"
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 10
#define HEIGHT 20

#define MAX_Y (HEIGHT - 1)
#define MAX_X (WIDTH)

typedef enum {
    WAITING,   
    START,
    SPAWN,     
    MOVING,     
    SHIFTING, 
    ATTACHING, 
    GAME_OVER
} GameStatus;

typedef struct {
    CurrentFigure current;  
    int current_fig[4][4];
    CurrentFigure next;         
    GameInfo_t info;            
    bool game_over;             
    unsigned long last_update; 
    GameStatus status; 
} GameState;




void initGame();
void initField();
void rotateFigure();
void copy_matrix(int a[4][4], int b[4][4]);
int tryAttachingOthers();
GameState updateGameState();
void spawnNewFigure(CurrentFigure *fig);
void saveFigureDown();
void moveLeft();
void moveRight();
void moveDown();
int checkDownY();




void lol();
#endif