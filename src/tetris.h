/*!
\file
\brief Заголовочный файл с описанием классов

Данный файл содержит в себе определения основных 
классов, используемых в демонстрационной программе
*/
#ifndef TETRIS_H_
#define TETRIS_H_

#include "brick_game.h"
#include "figure.h"
#include "frontend.h"
#include "moving.h"
#include "shifting.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // usleep
#include <time.h> //srand time

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
    CLEANING_LINES,
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

GameState* getGameState(void);
void setGameState(GameState newState);


void initGame();
void initField();
GameState updateGameState();
void spawnNewFigure(CurrentFigure *fig);

void gameLoop(WINDOW *win);

void lol();
#endif