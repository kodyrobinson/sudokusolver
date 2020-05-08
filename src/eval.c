#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "eval.h"

#define BOARDSIZE 9

static bool dup(elem *e);

bool boxeval(elem *e) {
    elem duparray[BOARDSIZE];
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            strcpy((char *)&duparray[3 * y + x], (const char *)(e + 9 * x + y));
        }
    }
    return !dup(duparray);

}

bool roweval(elem *e) {
    elem duparray[BOARDSIZE];
    for (int i = 0; i < BOARDSIZE; i++) {
        strcpy((char *)&duparray[i], (const char *)(e + i));
    }
    return !dup(duparray);
}

bool coleval(elem *e) {
    elem duparray[BOARDSIZE];
    for (int i = 0; i < BOARDSIZE; i++) {
        strcpy((char *)&duparray[i], (const char *)(e + i * BOARDSIZE));
    }
    return !dup(duparray);
}

// Static helper function returns true if the parameter array contains any duplicate values other than 0
static bool dup(elem *e) {
    val temparr[BOARDSIZE] = { 0 };
    for (int i = 0; i < BOARDSIZE; i++) {
        if (e->known) {
            temparr[i] = e[i].d.value;
        } else {
            temparr[i] = 0;
        }
    }
    // Bubble sort the temp array.
    for (int i = 0; i < BOARDSIZE-1; i++) {
        for (int n = i; n < BOARDSIZE-1; n++) {
            if (temparr[n] > temparr[n+1]) {
                int temp = temparr[n];
                temparr[n] = temparr[n+1];
                temparr[n+1] = temp;
            }
        }
    }
    // Perform a linear search to detect duplicates.
    for (int i = 0; i < BOARDSIZE-1; i++) {
        if (temparr[i] == temparr[i+1] && temparr[i] != 0) {
            return true;
        }
    }
    return false;
}
