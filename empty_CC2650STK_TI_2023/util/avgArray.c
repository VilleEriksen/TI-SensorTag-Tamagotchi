/*
 * avgArray.c
 *
 *  Created on: 30 Oct 2023
 *      Author: Ville
 */

#include <stdlib.h>
#include "avgArray.h"

struct avgArray* createAvgArray(int size, float startVal) {
    // Create a pointer to the array, and allocate memory for it.
    struct avgArray* arr = (struct avgArray*)malloc(sizeof(struct avgArray));

    // Also allocate memory for the array inside the struct.
    arr->arr = (float*)malloc(sizeof(float) * size);

    int i;
    // Initialize the array with given values.
    for (i = 0; i < size; i++) {
        *(arr->arr + i) = startVal;
    }

    // Give the other variables default values.
    arr->avg = 0.0;
    arr->i = 0;
    arr->size = size;

    // Finally return the pointer to the array.
    return arr;
}


void freeAvgArray(struct avgArray* arr) {
    free(arr->arr);
    free(arr);
}

void updateAvgArray(struct avgArray *arr, float val) {
    // Read the value from array at i.
    *(arr->arr +  arr->i) = val;
    // Increment i by one.
    arr->i = (arr->i + 1) % arr->size;

    arr->avg = calcAvg(arr->arr, arr->size);
}

float calcAvg(float* arr, int size) {
    float avg = 0.0;
    int i;

    // Loop though the array and add to the average.
    for (i = 0; i < size; i++) {
        avg += *arr;
        arr++;
    }

    // Divide with the size to get the final average.
    avg /= (float)size;

    return avg;
}

