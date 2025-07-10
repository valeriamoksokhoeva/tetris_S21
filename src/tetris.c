#include "tetris.h"
static GameState game_state;

GameState* getGameState(void) {
    return &game_state;
}

void setGameState(GameState newState) {
    game_state = newState;
}

void initField() {
    game_state.info.field = (int**)malloc((HEIGHT + 2) * sizeof(int*));
    for (int i = 0; i < HEIGHT + 2; i++) {
        game_state.info.field[i] = (int*)malloc((WIDTH + 2) * sizeof(int));
        for (int j = 0; j < WIDTH + 2; j++) {
            if (i == 0 || i == HEIGHT + 1 || j == 0 || j == WIDTH + 1) {
                game_state.info.field[i][j] = 1; 
            } else {
                game_state.info.field[i][j] = 0; 
            }
        }
    }

    game_state.info.next = (int**)malloc(4 * sizeof(int*));
    for (int i = 0; i < 4; i++) {
        game_state.info.next[i] = (int*)malloc(4 * sizeof(int));
    }
}

void initGame() {
    game_state.status = START;
    initField();
    
    game_state.info.score = 0;
    game_state.info.high_score = 0;
    game_state.info.level = 1;
    game_state.info.speed = 1;
    game_state.info.pause = 0;
    game_state.game_over = false;

    // spawnNewFigure(&game_state.current);
    spawnNewFigure(&game_state.next);

    // const FigureTypeProperties* curr_fig = &figures[game_state.current.figure_type];
    // for (int y = 0; y < 4; y++) {
    //     for (int x = 0; x < 4; x++) {
    //         game_state.current_fig[y][x] = curr_fig->blocks[y][x];
    //     }
    // }

    const FigureTypeProperties* next_fig = &figures[game_state.next.figure_type];
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            game_state.info.next[y][x] = next_fig->blocks[y][x];
        }
    }
}

void spawnNewFigure(CurrentFigure *fig){
    fig->x = WIDTH / 2 - 1;
    fig->y = 0;
    fig->figure_type = rand() % 7;
    game_state.status = SPAWN;
}

GameInfo_t updateCurrentState(){
    return game_state.info;
}

GameState updateGameState(){
    return game_state;
}

void gameLoop(WINDOW *win) {
    int frame_count = 0;
    const int frames_per_drop = 10;  
    while (game_state.status != GAME_OVER) {
        werase(win);
        get_inputs();
        if (frame_count % frames_per_drop == 0) {
            switch (game_state.status) {
                case WAITING:
                    break;
                    
                case START:
                    initGame();
                    game_state.status = SPAWN;
                    break;
                    
                case SPAWN:
                    spawnNewFigure(&game_state.current);
                    const FigureTypeProperties* curr_fig = &figures[game_state.current.figure_type];
                    for (int y = 0; y < 4; y++) {
                        for (int x = 0; x < 4; x++) {
                            game_state.current_fig[y][x] = curr_fig->blocks[y][x];
                        }
                    }
                    game_state.status = MOVING;
                    break;
                    
                case MOVING:
                    moveDown();
                    break;
                    
                case ATTACHING:
                    // fieldFullLine();
                    saveFigureDown();
                    // game_state.status = SPAWN;
                    break;
                case CLEANING_LINES:
                    fieldFullLine();
                    // saveFigureDown();
                    break;
                default:
                    break;
            }
        }
        box(win, 0, 0);
        drawPointField(win, game_state.info.field);
        if (game_state.status != WAITING) {
            drawFigure(win, game_state.current_fig, game_state.current.x, game_state.current.y);
        }
        wrefresh(win);
        
        usleep(100000); 
        frame_count++;
        if (frame_count >= frames_per_drop) {
            frame_count = 0;
        }
    }
}

void userInput(UserAction_t action) {
    switch (action) {
        case Start:
            if (game_state.status == WAITING || game_state.status == GAME_OVER) {
                game_state.status = START;
            }
            break;
            
        case Pause:
            if (game_state.status == MOVING) {
                game_state.status = WAITING;
            } else if (game_state.status == WAITING) {
                game_state.status = MOVING;
            }
            break;
            
        case Terminate:
            game_state.status = GAME_OVER;
            break;
            
        case Left:
            if (game_state.status == MOVING) moveLeft();
            break;
            
        case Right:
            if (game_state.status == MOVING) moveRight();
            break;
            
        case Up:
            // Можно использовать для ускоренного падения
            if (game_state.status == MOVING) {
                while (game_state.status == MOVING) {
                    moveDown();
                }
            }
            break;
            
        case Down:
            if (game_state.status == MOVING) moveDown();
            break;
            
        case Action:
            if (game_state.status == MOVING) rotateFigure();
            break;
    }
}

void lol() {
    WINDOW *win = startFront();
    game_state.status = WAITING;  // Начинаем в состоянии ожидания
    initGame();
    gameLoop(win);
    endwin();
}