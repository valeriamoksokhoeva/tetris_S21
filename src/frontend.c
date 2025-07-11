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
} // DO ENDWIN IN THE END

WINDOW* getWinNextFigure(){
    WINDOW *next_figure = newwin(5, 14, 12, 15);
    refresh();
    box(next_figure, 0, 0);
    mvwprintw(next_figure, 1, 1, "Next figure:");
    wrefresh(next_figure);
    getch();
    return next_figure;
}
void showButtons(){
    WINDOW *buttons = newwin(10, 21, 0, 15);
    refresh();
    box(buttons, 0, 0);
    mvwprintw(buttons, 1, 1, "Start = ENTER");
    mvwprintw(buttons, 2, 1, "Pause = 'p'");
    mvwprintw(buttons, 3, 1, "Terminate = 'q'");
    mvwprintw(buttons, 4, 1, "Left = left arrow");
    mvwprintw(buttons, 5, 1, "Right = right arrow");
    mvwprintw(buttons, 6, 1, "Down = down arrow");
    mvwprintw(buttons, 7, 1, "Up = up arrow");
    mvwprintw(buttons, 8, 1, "Action = SPACE");

    wrefresh(buttons);
    getch();
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
            userInput(Start, false);
        } else if (ch == KEY_LEFT){
            userInput(Left, false);
        } else if (ch == KEY_RIGHT){
            userInput(Right, false);
        } else if (ch == KEY_UP){
            userInput(Up, false);
        } else if (ch == KEY_DOWN){
            userInput(Down, false);
        } else if (ch == ' '){
            userInput(Action, false);
        } else if (ch == 'p' || ch == 'P'){
            userInput(Pause, false);
        } else if (ch == 'q' || ch == 'Q'){
            userInput(Terminate, false);
        }
        ch = getch();
    }
}

