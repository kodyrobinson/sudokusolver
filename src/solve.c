#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "eval.h"

#define BOARDSIZE 9
#define MAXFILESIZE 100
#define FILESIZE 162

static val board[BOARDSIZE][BOARDSIZE];

void printboard();
void initboard(char *boardfilename);

int main(int argc, const char *argv[]) {
    initboard((char *)argv[1]);
    printboard();
}

void printboard() {
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            printf("%d ", board[x][y]);
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
            procstring[procindex++] = boardstring[i] & 0b00001111;
        }
    }
    for (int i = 0; i < BOARDSIZE * BOARDSIZE; i++) {
        board[i % BOARDSIZE][(i - (i % BOARDSIZE)) / BOARDSIZE] = procstring[i];
    }
    free(boardstring);
    fclose(fp);
}
