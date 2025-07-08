#ifndef FIGURE_H_
#define FIGURE_H_

typedef enum {
    O_FIGURE = 0,
    I_FIGURE = 1,
    L_FIGURE = 2,
    J_FIGURE = 3,
    Z_FIGURE = 4,
    S_FIGURE = 5,
    T_FIGURE = 6
} FigureType;


typedef struct {
    int blocks[4][4];
} FigureTypeProperties;
extern const FigureTypeProperties figures[7];


typedef struct {
    int x;
    int y;
    FigureType figure_type;
} CurrentFigure;

#endif