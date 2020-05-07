#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "eval.h"

#define BOARDSIZE 9
#define MAXFILESIZE 100

static val board[BOARDSIZE][BOARDSIZE];

void printboard();

int main(int argc, const char *argv[]) {
    char *boardfilename = (char *) calloc(MAXFILESIZE, sizeof(char));
    strcpy(boardfilename, argv[1]);
    FILE *fp = fopen(boardfilename, "r");
    int numsread = 0;
    while (numsread < BOARDSIZE * BOARDSIZE) {
        char currchar = fgetc(fp);
        if (currchar >= 0 && currchar <= 9) {
            board[numsread % BOARDSIZE][(numsread - (numsread % BOARDSIZE)) / BOARDSIZE] = currchar;
            numsread++;
        }
        if (currchar == EOF) {
            break;
        }
    }
    free(boardfilename);
    printboard();
    
    fclose(fp);
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

