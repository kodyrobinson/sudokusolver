#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "eval.h"

#define BOARDSIZE 9

static val board[BOARDSIZE][BOARDSIZE];

void printboard();

int main() {
    printboard();
    printf("box eval: %d", boxeval(&board[0][0]));
    printf("row eval: %d", roweval(&board[0][0]));
}

void printboard() {
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            board[x][y] = 1;
            printf("%d ", board[x][y]);
        }
        printf("\n");
    }
}

