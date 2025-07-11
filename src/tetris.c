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
    // game_state.status = START;
    initField();
    
    game_state.info.score = 0;
    game_state.info.high_score = 0;
    game_state.info.level = 1;
    game_state.info.speed = 1;
    game_state.info.pause = 0;
    game_state.game_over = false;

    generateNextFigure();
}

void spawnNewFigure(CurrentFigure *fig){
    fig->x = WIDTH / 2 - 1;
    fig->y = 0;
    fig->figure_type = rand() % 7;
    // game_state.status = SPAWN;
}

GameInfo_t updateCurrentState(){
    return game_state.info;
}

GameState updateGameState(){
    return game_state;
}

void gameLoop(WINDOW *win) {
    int frame_count = 0;
    const int frames_per_drop = 7;  
    int no_curr = 0;
    while (1) {
        werase(win);
        get_inputs();

        if (frame_count % frames_per_drop == 0 || game_state.status == SPAWN || game_state.status == ATTACHING) {
            no_curr = 0;
            switch (game_state.status) {
                case WAITING:
                    break;
                case START:
                    no_curr = 1;
                    initGame();
                    game_state.status = SPAWN;
                    break;
                case SPAWN:
                    updateFigure();
                    generateNextFigure();
                    game_state.status = MOVING;
                    break;
                case MOVING:
                    moveDown();
                    break;
                case ATTACHING:
                    saveFigureDown();
                    break;
                case CLEANING_LINES:
                    no_curr = fieldFullLine();
                    break;
                case GAME_OVER:
                    cleanGameOver();
                    no_curr = 1;
                default:
                    break;
            }
        }
        box(win, 0, 0);
        if (game_state.status != WAITING) showGame(win, no_curr, game_state.info.field, game_state.current_fig, game_state.current.x, game_state.current.y, game_state.info.next);
        usleep(10000*7); 
        frame_count++;
        if (frame_count >= frames_per_drop) {
            frame_count = 0;
        }
    }
}

void userInput(UserAction_t action, bool hold) {
    (void)hold;
    switch (action) {
        case Start:
            if (game_state.status == GAME_OVER) {
                game_state.status = START;
            } else if (game_state.status == WAITING){
                game_state.status = SPAWN;
            }
            break;
            
        case Pause:
            if (game_state.status == MOVING || game_state.status == ATTACHING) {
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
            break;
            
        case Down:
            if (game_state.status == MOVING) moveDown();
            break;
            
        case Action:
            if (game_state.status == MOVING) rotateFigure();
            break;
    }
}

void play() {
    WINDOW *win = startFront();
    showButtons();
    // showNextFigure();
    // game_state.status = WAITING;  // Начинаем в состоянии ожидания
    initGame();
    game_state.status = WAITING;  // Начинаем в состоянии ожидания

    gameLoop(win);
    free(game_state.info.field);
    free(game_state.info.next);
    endwin();
}

// void showNextFigure(){
//     WINDOW *win = getWinNextFigure();
//     for (int i = 0; i < 4; i++) {
//         for (int j = 0; j < 4; j++) {
//             wmove(win, i+1, j+1);
//             if (game_state.info.next[i][j]){
//                 wprintw(win, "*");
//             } 
            
//         }
//     }
//     wrefresh(win);
// }

void generateNextFigure(){
    spawnNewFigure(&game_state.next);
    const FigureTypeProperties* next_fig = &figures[game_state.next.figure_type];
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            game_state.info.next[y][x] = next_fig->blocks[y][x];
        }
    }
}

void updateFigure(){
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            game_state.current_fig[y][x] = game_state.info.next[y][x];
        }
    }
    game_state.current.x = game_state.next.x;
    game_state.current.y = game_state.next.y;
    game_state.current.figure_type = game_state.next.figure_type;
}

// void showGame(WINDOW *win, int no_curr){
//     drawPointField(win, game_state.info.field);
//     if (!no_curr) {
//         drawFigure(win, game_state.current_fig, game_state.current.x, game_state.current.y);
//         no_curr = 0;
//      }
//     showNextFigure();
//     wrefresh(win);
// }

void cleanGameOver(){
    for (int i = 1; i <= HEIGHT; i++) {
        for (int j = 1; j <= WIDTH; j++) {
            game_state.info.field[i][j] = 0;
        }
    }
    for (int i = 0; i < 4; i++){
        for (int j = 0; j< 4; j++){
            game_state.info.next[i][j] = 0;
        }
    }
}