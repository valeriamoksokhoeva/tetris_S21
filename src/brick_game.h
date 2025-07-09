#ifndef BRICK_GAME_H_
#define BRICK_GAME_H_

#include <stdbool.h>

typedef enum UserAction {
    Start,
    Pause,
    Terminate,
    Left,
    Right,
    Up,
    Down,
    Action
} UserAction_t;

typedef struct GameInfo
{
    int **field;
    int **next;
    int score;
    int high_score;
    int level;
    int speed;
    int pause;
} GameInfo_t;

void userInput(UserAction_t action); //ADD HOLD

GameInfo_t updateCurrentState();



#endif