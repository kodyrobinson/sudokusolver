#include <stdlib.h>
#include <stdbool.h>

#include "eval.h"

#define BOARDSIZE 9

static bool dup(val *firstval);

bool boxeval(val *firstval) {
    return false;
}

bool roweval(val *firstval) {
    return !dup(firstval);
}

bool coleval(val *firstval) {
    val duparray[BOARDSIZE] = { 0 };
    for (int i = 0; i < BOARDSIZE; i++) {
        duparray[i] = firstval[i * BOARDSIZE];
    }
    return !dup(duparray);
}

// Static helper function returns true if the parameter array contains any duplicate values other than 0
static bool dup(val *firstval) {
    val temparr[BOARDSIZE] = { 0 };
    for (int i = 0; i < BOARDSIZE; i++) {
        temparr[i] = firstval[i];
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
