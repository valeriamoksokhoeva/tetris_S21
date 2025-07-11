#include "shifting.h"

int fieldFullLine(){
    GameState* gs = getGameState();
    int no_curr = 0;
    for (int i = 1; i< HEIGHT+1; i++){
        int full = 1;
        for (int j = 1; j < WIDTH+1; j++){
            if (!gs->info.field[i][j]) full = 0;
        }
        if (full){
            for (int j = 1; j <WIDTH+1; j++){
                gs->info.field[i][j] = 0;
                if (gs->current_fig[i+gs->current.y][j+gs->current.x]) {
                    gs->current_fig[i+gs->current.y][j+gs->current.x] = 0;
                }
            }
            fallDownField(i);
            no_curr = 1;
        } 
    }
    // usleep(989);
    if (gameOverLines()) {
        gs->status = GAME_OVER;
    } else {
        gs->status = SPAWN;
    }
    return no_curr;
}

int haveFullLine(){
    GameState* gs = getGameState();
    int sum_lines = 0;
    for (int i = 1; i< HEIGHT+1; i++){
        int full = 1;
        for (int j = 1; j < WIDTH+1; j++){
            if (!gs->info.field[i][j]) full = 0;
        }
        if (full) sum_lines++;
    }
    return sum_lines; // 1 means there are full lines, 0 means no full lines
}

void fallDownField(int line){ // go down , after cleaning line 
    GameState* gs = getGameState();
    for (int i = line; i > 1; i--){
        for (int j = 0; j< WIDTH + 2; j++){
            gs->info.field[i][j] = gs->info.field[i-1][j];
        }
    }
    for (int j = 1; j < WIDTH + 1; j++){
        gs->info.field[1][j] = 0;
    }
}

int gameOverLines(){
    GameState* gs = getGameState();
    int game_over = 0;
    for (int i = 4; i < 9; i++){
        if (gs->info.field[1][i]) game_over = 1;
    }
    return game_over;
}