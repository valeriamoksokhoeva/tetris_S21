#include "frontend.h"

WINDOW* startFront(){
    initscr();
    curs_set(0);
    cbreak(); 
    noecho(); 
    keypad(stdscr, TRUE); 
    nodelay(stdscr, TRUE);

    WINDOW *win = newwin(HEIGHT + 2, WIDTH + 2, 0, 0);
    refresh();

    box(win, 0, 0);
    wrefresh(win);

    return win;
}

void drawPointField(WINDOW *win, int **field){ //draw empty field ( no figures )
    for (int i = 1; i < WIDTH+1; i++){
        for (int j = 1; j < HEIGHT+1; j++){
            wmove(win,j, i);
            if (field[j][i]){
                wprintw(win, "*");
            } else {
                wprintw(win, ".");
            }
        }
    }
    // wrefresh(win);
}

void drawFigure(WINDOW *win, int figure[4][4], int x, int y){
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            wmove(win, y + i, x + j);
            if (figure[i][j]){
                wprintw(win, "*");
            } 
            
        }
    }
    // wrefresh(win);
}

void get_inputs(){
    int ch = getch();
    while (ch != ERR) { 
        if (ch == '\n'){
            userInput(Start);
        } else if (ch == KEY_LEFT){
            userInput(Left);
        } else if (ch == KEY_RIGHT){
            userInput(Right);
        } else if (ch == KEY_UP){
            userInput(Up);
        } else if (ch == KEY_DOWN){
            userInput(Down);
        } else if (ch == ' '){
            userInput(Action);
        } else if (ch == 'p' || ch == 'P'){
            userInput(Pause);
        } else if (ch == 'q' || ch == 'Q'){
            userInput(Terminate);
        }
        ch = getch();
    }
}

