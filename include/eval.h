#ifndef EVAL_H
#define EVAL_H

#include <stdlib.h>

// This file is will contain the declaration for functions that will be used to evaluate whether the current state of the sudoku puzzle conforms to the rules of sudoku.

typedef char val;

typedef union data {
    val value;
    bool potvals[10];
} data;

typedef struct elem {
    data d;
    bool known;
} elem;

// The following function takes as a parameter a pointer to the first value of a 3x3 box on a sudoku board.
// It will return true if there are no duplicate values within that box, false otherwise.
bool boxeval(val *firstval);

// The following function takes as a parameter a pointer to the first value of a 9x1 row on a sudoku board.
// It will return true if there are no duplicate values within that box, false otherwise.
bool roweval(val *firstval);

// The following function takes as a parameter a pointer to the first value of a 1x9 column on a sudoku board.
// It will return true if there are no duplicate values within that box, false otherwise.
bool coleval(val *firstval);

#endif
