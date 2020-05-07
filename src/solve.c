#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "eval.h"

#define BOARDSIZE 9
#define MAXFILESIZE 100
#define FILESIZE 162
#define MASK 0b00001111

static elem board[BOARDSIZE][BOARDSIZE];

void printboard();
void initboard(char *boardfilename);

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "You should call this function and add as an argument the path to a file with an unsolved sudoku puzzle.\n");
        return EXIT_FAILURE;
    }
    initboard((char *)argv[1]);
    printboard();
}

void printboard() {
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            if (board[x][y].known) {
                printf("%d ", board[x][y].d.value);
            } else {
                printf("0 ");
            }
        }
        printf("\n");
    }
}

void initboard(char *boardfilename) {
    FILE *fp = fopen(boardfilename, "r");
    char *boardstring = (char *) calloc(FILESIZE, sizeof(char));
    fgets(boardstring, FILESIZE, fp);
    char procstring[BOARDSIZE * BOARDSIZE] = { 0 };
    int procindex = 0;
    for (int i = 0; i < FILESIZE; i++) {
        if (boardstring[i] >= '0' && boardstring[i] <= '9') {
            procstring[procindex++] = boardstring[i] & MASK;
        }
    }
    for (int i = 0; i < BOARDSIZE * BOARDSIZE; i++) {
        if (procstring[i] == 0) {
            board[i % BOARDSIZE][(i - (i % BOARDSIZE)) / BOARDSIZE].known = false;
            bool potentials[10] = { true };
            strcpy((char *)board[i % BOARDSIZE][(i - (i % BOARDSIZE)) / BOARDSIZE].d.potvals, (const char *)potentials);
        } else {
            board[i % BOARDSIZE][(i - (i % BOARDSIZE)) / BOARDSIZE].known = true;
            board[i % BOARDSIZE][(i - (i % BOARDSIZE)) / BOARDSIZE].d.value = procstring[i];
        }
    }
    free(boardstring);
    fclose(fp);
}
