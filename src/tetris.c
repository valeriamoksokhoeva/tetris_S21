#include "tetris.h"

static GameState game_state;
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

    spawnNewFigure(&game_state.current);
    spawnNewFigure(&game_state.next);

    const FigureTypeProperties* curr_fig = &figures[game_state.current.figure_type];
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            game_state.current_fig[y][x] = curr_fig->blocks[y][x];
        }
    }

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
    srand(time(NULL));
    fig->figure_type = 4;
    game_state.status = SPAWN;
}

GameInfo_t updateCurrentState(){
    return game_state.info;
}

GameState updateGameState(){
    return game_state;
}


void rotateFigure(){
    int success = 1;
    int prev_figure[4][4];
    copy_matrix(game_state.current_fig, prev_figure);
    int dubFigure[4][4];

    for (int i =0; i<4; i++){
        for (int j=0; j<4; j++){
            dubFigure[i][j] = game_state.current_fig[i][j];
        }
    }
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            game_state.current_fig[3-j][i] = dubFigure[i][j];
        }
    }

    //looking for intersect figure with borders
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (game_state.current_fig[i][j] && game_state.info.field[game_state.current.y + i][game_state.current.x + j]) success = 0;
        } 
    }

    if (!success) copy_matrix(prev_figure, game_state.current_fig); 
}

void copy_matrix(int a[4][4], int b[4][4]){
    for (int i=0; i<4;i++){
        for (int j =0; j<4; j++){
            b[i][j] = a[i][j];
        }
    }
}

int tryAttachingOthers(){ //check if we intersect with saved figures on field
    int success = 1;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (game_state.current_fig[i][j] && game_state.info.field[game_state.current.y + i][game_state.current.x + j]){
                success = 0;
            }
        }    
    }
    return success;
}

void moveLeft(){
    game_state.current.x--;
    if (!tryAttachingOthers()) game_state.current.x++; // if not success, come back to prev position
}

void moveRight(){
    game_state.current.x++;
    if (!tryAttachingOthers()) game_state.current.x--;
}

void moveDown(){
    game_state.current.y++;
    int linesEmpty = checkDownY();
    if ((game_state.current.y + (3 - linesEmpty) > MAX_Y) || (!tryAttachingOthers())){
        game_state.current.y--;
        game_state.status = ATTACHING;
    }
}

int checkDownY(){
    int linesEmptyDown = 0;
    int flagFull = 0;
    for (int i = 3; i >= 0; i--){
        for (int j = 0; j<4; j++){
            if (game_state.current_fig[i][j]) flagFull = 1;
        }
        if (!flagFull){
            linesEmptyDown++;
        }
    }
    return linesEmptyDown; //returns how many empty lines from down we have in matrix figure
}

void saveFigureDown(){ //x,y - coordinates of upper left point of matrix 4*4 for figure
    int x = game_state.current.x; //ACTUALLLY THIS IS ATTACHING 
    int y = game_state.current.y;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (game_state.current_fig[i][j]) game_state.info.field[y + i][x + j] = game_state.current_fig[i][j]; //to avoid intersection of matrixes figures
        } 
            
    }
    game_state.status = SPAWN;
} //AFTER THAT GO TO SPAWN


void lol() {
    WINDOW *win = startFront();
    initGame();
    gameLoop(win);
    endwin();
}


void userInput(UserAction_t action){
    switch (action)
    {
    case Start:
        printf("start");
        if (game_state.status == WAITING || game_state.status == GAME_OVER){
            initGame();
        }
        break;
    case Pause:
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
void gameLoop(WINDOW *win) {

    while (1) {
        werase(win);  
        get_inputs();
        moveDown(); 
        box(win, 0, 0);
        drawPointField(win, game_state.info.field);
        drawFigure(win, game_state.current_fig, game_state.current.x, game_state.current.y);
        wrefresh(win);  

        usleep(100000);  // (~0.1 сек, можно регулировать)
    }
}
