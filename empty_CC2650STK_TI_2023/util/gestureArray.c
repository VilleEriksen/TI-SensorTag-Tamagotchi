/*
 * gestureArray.c
 *
 *  Created on: 30 Oct 2023
 *      Author: Ville
 */

#include <stdlib.h>
#include <util/gestureArray.h>
#include "Board.h"

struct gestureArray* createGestureArray(uint8_t size, enum gestures startGesure) {
    // Create a pointer to the array, and allocate memory for it.
    struct gestureArray* arr = (struct gestureArray*)malloc(sizeof(struct gestureArray));

    // Also allocate memory for the array inside the struct.
    arr->arr = (enum gestures*)malloc(sizeof(enum gestures) * size);

    uint8_t i;
    // Initialize the array with given values.
    for (i = 0; i < size; i++) {
        *(arr->arr + i) = startGesure;
    }

    // Give the other variables default values.
    arr->avg = startGesure;
    arr->i = 0;
    arr->size = size;

    // Finally return the pointer to the array.
    return arr;
}


void freeGestureArray(struct gestureArray* arr) {
    free(arr->arr);
    free(arr);
}

void updateGestureArray(struct gestureArray *arr, enum gestures gesture) {
    // Read the value from array at i.
    *(arr->arr +  arr->i) = gesture;
    // Increment i by one.
    arr->i = (arr->i + 1) % arr->size;

    arr->avg = calcGestureArrayAvg(arr->arr, arr->size);
}

enum gestures calcGestureArrayAvg(enum gestures* arr, uint8_t size) {
    enum gestures avg = NONE;
    uint8_t maxCount = 0;
    uint8_t count;
    uint8_t i;
    uint8_t j;

    // Loop though the elements in the array.
    for (i = 0; i < size; i++) {
        count = 0;

        // Check the element against others and add to the count of they match.
        for (j = 0; j < size; j++) {
            if ((arr + i) == (arr + j)) count++;
        }

        if (maxCount < count) {
            maxCount = count;
            avg = *(arr + i);
        }
    }

    return avg;
}

bool gestureArrayContains(struct gestureArray* arr, enum gestures gesture) {
    uint8_t i;

    for (i = 0; i < arr->size; i++) {
        if (*(arr->arr + i) == gesture) return true;
    }

    return false;
}

