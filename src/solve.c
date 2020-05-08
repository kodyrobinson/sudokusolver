#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "eval.h"

#define BOARDSIZE 9
#define MAXFILESIZE 1000
#define FILESIZE 162
#define MASK 0b00001111

typedef struct point {
    int x;
    int y;
} point;

static elem board[BOARDSIZE][BOARDSIZE];
int unknownleft = BOARDSIZE * BOARDSIZE;

void printboard(elem (*map)[BOARDSIZE][BOARDSIZE]);
void initboard(char *boardfilename);
bool findpotvals(elem (*map)[BOARDSIZE][BOARDSIZE], int x, int y, int pots);
bool evalboard(elem board[BOARDSIZE][BOARDSIZE]);

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "You should call this function and add as an argument the path to a file with an unsolved sudoku puzzle.\n");
        return EXIT_FAILURE;
    }
    initboard((char *)argv[1]);
    printboard(&board);
    printf("=================\n");
    for (int y = 0; y < BOARDSIZE; y++) {
        for (int x = 0; x < BOARDSIZE; x++) {
            if (board[x][y].known) {
                continue;
            }
            if (unknownleft == 0) {
                break;
            }
            findpotvals(&board, x, y, 1);
        }
    }
    if (unknownleft > 0) {
        for (int y = 0; y < BOARDSIZE; y++) {
            for (int x = 0; x < BOARDSIZE; x++) {
                if (board[x][y].known) {
                    continue;
                }
                if (unknownleft == 0) {
                    break;
                }
                findpotvals(&board, x, y, 3);
            }
            if (unknownleft == 0) {
                break;
            }
        }
    }
    printboard(&board);
    printf("=================\n");
    printf("The number of unsolved spaces are: %d\n", unknownleft);
    if (evalboard(board) && unknownleft == 0) {
        printf("Board has been solved.\n");
    } else {
        printf("An error occurred and the board was not solved properly.\n");
    }
}

void printboard(elem (*map)[BOARDSIZE][BOARDSIZE]) {
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            if ((*map)[x][y].known) {
                printf("%d ", (*map)[x][y].d.value);
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
            bool potentials[10] = {true, true, true, true, true, true, true, true, true, true};
            strcpy((char *)board[i % BOARDSIZE][(i - (i % BOARDSIZE)) / BOARDSIZE].d.potvals, (const char *)potentials);
        } else {
            unknownleft--;
            board[i % BOARDSIZE][(i - (i % BOARDSIZE)) / BOARDSIZE].known = true;
            board[i % BOARDSIZE][(i - (i % BOARDSIZE)) / BOARDSIZE].d.value = procstring[i];
        }
    }
    free(boardstring);
    fclose(fp);
    if (evalboard(board)) {
        printf("Board looks good!\n");
    } else {
        printf("It appears the board has some errors.\n");
    }
}

bool findpotvals(elem (*map)[BOARDSIZE][BOARDSIZE], int x, int y, int pots) {
    if ((*map)[x][y].known || unknownleft == 0) {
        return false;
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
        if ((*map)[box[i].x][box[i].y].known) {
            (*map)[x][y].d.potvals[(int)(*map)[box[i].x][box[i].y].d.value] = false;
        }
        if ((*map)[row[i].x][row[i].y].known) {
            (*map)[x][y].d.potvals[(int)(*map)[row[i].x][row[i].y].d.value] = false;
        }
        if ((*map)[col[i].x][col[i].y].known) {
            (*map)[x][y].d.potvals[(int)(*map)[col[i].x][col[i].y].d.value] = false;
        }
    }
    int potentials = 0;
    for (int i = 1; i < 10; i++) {
        if ((*map)[x][y].d.potvals[i]) {
            potentials++;
        }
    }
    if (potentials == 1) {
        int i = 1;
        while (!(*map)[x][y].d.potvals[i]) {
            i++;
        }
        unknownleft--;
        (*map)[x][y].known = true;
        (*map)[x][y].d.value = i;
        for (int i = 0; i < BOARDSIZE; i++) {
            findpotvals(map, box[i].x, box[i].y, 1);
            findpotvals(map, row[i].x, row[i].y, 1);
            findpotvals(map, col[i].x, col[i].y, 1);
        }
    }
    if (potentials == 2 && pots >= 2) {
        int first = 1;
        int second = 1;
        while (!(*map)[x][y].d.potvals[first]) {
            first++;
        }
        second = first + 1;
        while (!(*map)[x][y].d.potvals[second]) {
            second++;
        }
        elem (*tempboard)[BOARDSIZE][BOARDSIZE] = calloc(BOARDSIZE * BOARDSIZE, sizeof(elem));
        for (int i = 0; i < BOARDSIZE; i++) {
            for (int j = 0; j < BOARDSIZE; j++) {
                (*tempboard)[j][i].known = (*map)[j][i].known;
                (*tempboard)[j][i].d = (*map)[j][i].d;
            }
        }
        (*tempboard)[x][y].known = true;
        (*tempboard)[x][y].d.value = first;
        for (int i = 0; i < BOARDSIZE; i++) {
            findpotvals(tempboard, box[i].x, box[i].y, 1);
            findpotvals(tempboard, row[i].x, row[i].y, 1);
            findpotvals(tempboard, col[i].x, col[i].y, 1);
        }
        for (int i = 0; i < BOARDSIZE; i++) {
            findpotvals(tempboard, box[i].x, box[i].y, 2);
            findpotvals(tempboard, row[i].x, row[i].y, 2);
            findpotvals(tempboard, col[i].x, col[i].y, 2);
        }
        int tempcomplete = 0;
        for (int i = 0; i < BOARDSIZE * BOARDSIZE; i++) {
            if ((*tempboard)[i % BOARDSIZE][i - (i % BOARDSIZE)].known) {
                tempcomplete++;
            }
        }
        elem (*otherboard)[BOARDSIZE][BOARDSIZE] = calloc(BOARDSIZE * BOARDSIZE, sizeof(elem));
        for (int i = 0; i < BOARDSIZE; i++) {
            for (int j = 0; j < BOARDSIZE; j++) {
                (*otherboard)[j][i].known = (*map)[j][i].known;
                (*otherboard)[j][i].d = (*map)[j][i].d;
            }
        }
        (*otherboard)[x][y].known = true;
        (*otherboard)[x][y].d.value = second;
        for (int i = 0; i < BOARDSIZE; i++) {
            findpotvals(otherboard, box[i].x, box[i].y, 1);
            findpotvals(otherboard, row[i].x, row[i].y, 1);
            findpotvals(otherboard, col[i].x, col[i].y, 1);
        }
        for (int i = 0; i < BOARDSIZE; i++) {
            findpotvals(otherboard, box[i].x, box[i].y, 2);
            findpotvals(otherboard, row[i].x, row[i].y, 2);
            findpotvals(otherboard, col[i].x, col[i].y, 2);
        }
        int othercomplete = 0;
        for (int i = 0; i < BOARDSIZE * BOARDSIZE; i++) {
            if ((*otherboard)[i % BOARDSIZE][i - (i % BOARDSIZE)].known) {
                othercomplete++;
            }
        }
        if (tempcomplete > othercomplete) {
            unknownleft = 0;
            for (int i = 0; i < BOARDSIZE; i++) {
                for (int j = 0; j < BOARDSIZE; j++) {
                    (*map)[j][i].known = (*tempboard)[j][i].known;
                    (*map)[j][i].d = (*tempboard)[j][i].d;
                }
            }
        } else {
            unknownleft = 0;
            for (int i = 0; i < BOARDSIZE; i++) {
                for (int j = 0; j < BOARDSIZE; j++) {
                    (*map)[j][i].known = (*otherboard)[j][i].known;
                    (*map)[j][i].d = (*otherboard)[j][i].d;
                }
            }
        }
        free(tempboard);
        free(otherboard);
    }

    if (potentials == 3 && pots == 3) {
        int first = 1;
        int second = 1;
        int third = 1;
        while (!(*map)[x][y].d.potvals[first]) {
            first++;
        }
        second = first + 1;
        while (!(*map)[x][y].d.potvals[second]) {
            second++;
        }
        third = second + 1;
        while (!(*map)[x][y].d.potvals[third]) {
            third++;
        }
        elem (*tempboard)[BOARDSIZE][BOARDSIZE] = calloc(BOARDSIZE * BOARDSIZE, sizeof(elem));
        for (int i = 0; i < BOARDSIZE; i++) {
            for (int j = 0; j < BOARDSIZE; j++) {
                (*tempboard)[j][i].known = (*map)[j][i].known;
                (*tempboard)[j][i].d = (*map)[j][i].d;
            }
        }
        (*tempboard)[x][y].known = true;
        (*tempboard)[x][y].d.value = first;
        for (int i = 0; i < BOARDSIZE; i++) {
            findpotvals(tempboard, box[i].x, box[i].y, 1);
            findpotvals(tempboard, row[i].x, row[i].y, 1);
            findpotvals(tempboard, col[i].x, col[i].y, 1);
        }
        for (int i = 0; i < BOARDSIZE; i++) {
            findpotvals(tempboard, box[i].x, box[i].y, 2);
            findpotvals(tempboard, row[i].x, row[i].y, 2);
            findpotvals(tempboard, col[i].x, col[i].y, 2);
        }
        for (int i = 0; i < BOARDSIZE; i++) {
            findpotvals(tempboard, box[i].x, box[i].y, 3);
            findpotvals(tempboard, row[i].x, row[i].y, 3);
            findpotvals(tempboard, col[i].x, col[i].y, 3);
        }
        int tempcomplete = 0;
        for (int i = 0; i < BOARDSIZE * BOARDSIZE; i++) {
            if ((*tempboard)[i % BOARDSIZE][i - (i % BOARDSIZE)].known) {
                tempcomplete++;
            }
        }
        elem (*otherboard)[BOARDSIZE][BOARDSIZE] = calloc(BOARDSIZE * BOARDSIZE, sizeof(elem));
        for (int i = 0; i < BOARDSIZE; i++) {
            for (int j = 0; j < BOARDSIZE; j++) {
                (*otherboard)[j][i].known = (*map)[j][i].known;
                (*otherboard)[j][i].d = (*map)[j][i].d;
            }
        }
        (*otherboard)[x][y].known = true;
        (*otherboard)[x][y].d.value = second;
        for (int i = 0; i < BOARDSIZE; i++) {
            findpotvals(otherboard, box[i].x, box[i].y, 1);
            findpotvals(otherboard, row[i].x, row[i].y, 1);
            findpotvals(otherboard, col[i].x, col[i].y, 1);
        }
        for (int i = 0; i < BOARDSIZE; i++) {
            findpotvals(otherboard, box[i].x, box[i].y, 2);
            findpotvals(otherboard, row[i].x, row[i].y, 2);
            findpotvals(otherboard, col[i].x, col[i].y, 2);
        }
        for (int i = 0; i < BOARDSIZE; i++) {
            findpotvals(otherboard, box[i].x, box[i].y, 3);
            findpotvals(otherboard, row[i].x, row[i].y, 3);
            findpotvals(otherboard, col[i].x, col[i].y, 3);
        }
        int othercomplete = 0;
        for (int i = 0; i < BOARDSIZE * BOARDSIZE; i++) {
            if ((*otherboard)[i % BOARDSIZE][i - (i % BOARDSIZE)].known) {
                othercomplete++;
            }
        }
        elem (*thirdboard)[BOARDSIZE][BOARDSIZE] = calloc(BOARDSIZE * BOARDSIZE, sizeof(elem));
        for (int i = 0; i < BOARDSIZE; i++) {
            for (int j = 0; j < BOARDSIZE; j++) {
                (*thirdboard)[j][i].known = (*map)[j][i].known;
                (*thirdboard)[j][i].d = (*map)[j][i].d;
            }
        }
        (*thirdboard)[x][y].known = true;
        (*thirdboard)[x][y].d.value = third;
        for (int i = 0; i < BOARDSIZE; i++) {
            findpotvals(thirdboard, box[i].x, box[i].y, 1);
            findpotvals(thirdboard, row[i].x, row[i].y, 1);
            findpotvals(thirdboard, col[i].x, col[i].y, 1);
        }
        for (int i = 0; i < BOARDSIZE; i++) {
            findpotvals(thirdboard, box[i].x, box[i].y, 2);
            findpotvals(thirdboard, row[i].x, row[i].y, 2);
            findpotvals(thirdboard, col[i].x, col[i].y, 2);
        }
        for (int i = 0; i < BOARDSIZE; i++) {
            findpotvals(thirdboard, box[i].x, box[i].y, 3);
            findpotvals(thirdboard, row[i].x, row[i].y, 3);
            findpotvals(thirdboard, col[i].x, col[i].y, 3);
        }
        int thirdcomplete = 0;
        for (int i = 0; i < BOARDSIZE * BOARDSIZE; i++) {
            if ((*thirdboard)[i % BOARDSIZE][i - (i % BOARDSIZE)].known) {
                thirdcomplete++;
            }
        }
        if (tempcomplete > othercomplete && tempcomplete > thirdcomplete) {
            unknownleft = 0;
            for (int i = 0; i < BOARDSIZE; i++) {
                for (int j = 0; j < BOARDSIZE; j++) {
                    (*map)[j][i].known = (*tempboard)[j][i].known;
                    (*map)[j][i].d = (*tempboard)[j][i].d;
                }
            }
        } else if (othercomplete > tempcomplete && othercomplete > thirdcomplete) {
            unknownleft = 0;
            for (int i = 0; i < BOARDSIZE; i++) {
                for (int j = 0; j < BOARDSIZE; j++) {
                    (*map)[j][i].known = (*otherboard)[j][i].known;
                    (*map)[j][i].d = (*otherboard)[j][i].d;
                }
            }
        } else {
            unknownleft = 0;
            for (int i = 0; i < BOARDSIZE; i++) {
                for (int j = 0; j < BOARDSIZE; j++) {
                    (*map)[j][i].known = (*thirdboard)[j][i].known;
                    (*map)[j][i].d = (*thirdboard)[j][i].d;
                }
            }
        free(thirdboard);
        free(tempboard);
        free(otherboard);
        }

    }

    return true;
}

bool evalboard(elem map[BOARDSIZE][BOARDSIZE]) {
    // The goal is to include another parameter in the findpotvals function so
    // that it can check for values with a potentials value of 1 first. This 
    // will not always work with every puzzle, so we have to also check for 
    // spaces with a potential value of two. Take the first of these values and
    // allocate mem for a new board, copy the original board and set that space 
    // to one of the 2 possibilites, then let it naturally cause a chain reaction
    // and use this to see if the board is legit. If so, make the real board copy
    // the forked board. If not, set the value to be the other possibility and 
    // carry on. After this is performed, you have to revert to looking for 
    // potentials with a value of 1 because the new value will change the box, 
    // row, and column.

    for (int i = 0; i < BOARDSIZE; i++) {
        if (!coleval(&map[i][0])) {
            return false;
        }
        if (!roweval(&map[0][i])) {
            return false;
        }
        if (!boxeval(&map[i % 3][i - (i % 3)])) {
            return false;
        }
    }
    return true;
}








