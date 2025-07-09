#include "moving.h"

void rotateFigure() {
    GameState* gs = getGameState();
    int success = 1;
    int prev_figure[4][4];
    copy_matrix(gs->current_fig, prev_figure);
    int dubFigure[4][4];

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            dubFigure[i][j] = gs->current_fig[i][j];
        }
    }
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            gs->current_fig[3-j][i] = dubFigure[i][j];
        }
    }

    // Проверка коллизий после поворота
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (gs->current_fig[i][j] && gs->info.field[gs->current.y + i][gs->current.x + j]) {
                success = 0;
            }
        } 
    }

    if (!success) copy_matrix(prev_figure, gs->current_fig); 
}

void copy_matrix(int a[4][4], int b[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            b[i][j] = a[i][j];
        }
    }
}

int tryAttachingOthers() {
    GameState* gs = getGameState();
    int success = 1;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (gs->current_fig[i][j] && gs->info.field[gs->current.y + i][gs->current.x + j]) {
                success = 0;
            }
        }    
    }
    return success;
}

void moveLeft() {
    GameState* gs = getGameState();
    gs->current.x--;
    if (!tryAttachingOthers()) gs->current.x++;
}

void moveRight() {
    GameState* gs = getGameState();
    gs->current.x++;
    if (!tryAttachingOthers()) gs->current.x--;
}

void moveDown() {
    GameState* gs = getGameState();
    gs->current.y++;
    int linesEmpty = checkDownY();
    
    if ((gs->current.y + (2 - linesEmpty) > MAX_Y) || (!tryAttachingOthers())) {
        gs->current.y--;
        gs->status = ATTACHING;
    }
}

int checkDownY() {
    GameState* gs = getGameState();
    int linesEmptyDown = 0;
    int flagFull = 0;
    
    for (int i = 3; i >= 0; i--) {
        for (int j = 0; j < 4; j++) {
            if (gs->current_fig[i][j]) flagFull = 1;
        }
        if (!flagFull) {
            linesEmptyDown++;
        }
    }
    return linesEmptyDown;
}

void saveFigureDown() {
    GameState* gs = getGameState();
    int x = gs->current.x;
    int y = gs->current.y;
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (gs->current_fig[i][j]) {
                gs->info.field[y + i][x + j] = gs->current_fig[i][j];
            }
        } 
    }
    if (haveFullLine()) {
        gs->status = CLEANING_LINES;
    } else {
        gs->status = SPAWN;
    }
}