#include "shifting.h"

void fieldFullLine(){
    GameState* gs = getGameState();
    for (int i = 1; i< HEIGHT+1; i++){
        int full = 1;
        for (int j = 1; j < WIDTH+1; j++){
            if (!gs->info.field[i][j]) full = 0;
        }
        if (full){
            // for (int j = 1; j <WIDTH+1; j++){
            //     gs->info.field[i][j] = 0;
            // }
            fallDownField(i);
        }
    }
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