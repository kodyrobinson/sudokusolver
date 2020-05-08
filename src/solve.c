#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "eval.h"

#define BOARDSIZE 9
#define MAXFILESIZE 100
#define FILESIZE 162
#define MASK 0b00001111

typedef struct point {
    int x;
    int y;
} point;

static elem board[BOARDSIZE][BOARDSIZE];
int unknownleft = BOARDSIZE * BOARDSIZE;

void printboard();
void initboard(char *boardfilename);
void findpotvals(int x, int y);

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "You should call this function and add as an argument the path to a file with an unsolved sudoku puzzle.\n");
        return EXIT_FAILURE;
    }
    initboard((char *)argv[1]);
    printboard();
    printf("=================\n");
    for (int y = 0; y < BOARDSIZE; y++) {
        for (int x = 0; x < BOARDSIZE; x++) {
            findpotvals(x, y);
        }
    }
    printboard();
    return EXIT_SUCCESS;
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
            unknownleft--;
            board[i % BOARDSIZE][(i - (i % BOARDSIZE)) / BOARDSIZE].known = true;
            board[i % BOARDSIZE][(i - (i % BOARDSIZE)) / BOARDSIZE].d.value = procstring[i];
        }
    }
    free(boardstring);
    fclose(fp);
}

void findpotvals(int x, int y) {
    if (board[x][y].known) {
        return;
    }
    point box[BOARDSIZE];
    point row[BOARDSIZE];
    point col[BOARDSIZE];
    for (int i = 0; i < BOARDSIZE; i++) {
        point boxpt = {(i % 3) + x - (x % 3), ((i - (i % 3)) / 3) + y - (y % 3)};
        box[i] = boxpt;
        point rowpt = {i, y};
        row[i] = rowpt;
        point colpt = {x, i};
        col[i] = colpt;
    }
    for (int i = 0; i < BOARDSIZE; i++) {
        if (board[box[i].x][box[i].y].known) {
            board[x][y].d.potvals[(int)board[box[i].x][box[i].y].d.value] = false;
        }
        if (board[row[i].x][row[i].y].known) {
            board[x][y].d.potvals[(int)board[row[i].x][row[i].y].d.value] = false;
        }
        if (board[col[i].x][col[i].y].known) {
            board[x][y].d.potvals[(int)board[col[i].x][col[i].y].d.value] = false;
        }
    }
    int potentials = 0;
    for (int i = 1; i < 10; i++) {
        if (board[x][y].d.potvals[i]) {
            potentials++;
        }
    }
    if (potentials == 1) {
        int i = 1;
        while (!board[x][y].d.potvals[i]) {
            i++;
        }
        board[x][y].known = true;
        board[x][y].d.value = i;
        for (int i = 0; i < BOARDSIZE; i++) {
            findpotvals(box[i].x, box[i].y);
            findpotvals(row[i].x, row[i].y);
            findpotvals(col[i].x, col[i].y);
        }
    }

}









