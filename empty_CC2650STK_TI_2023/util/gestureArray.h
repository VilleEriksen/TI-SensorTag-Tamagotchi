/*
 * gestureArray.h
 *
 *  Created on: 8 Nov 2023
 *      Author: Ville
 */

#ifndef UTIL_GESTUREARRAY_H_
#define UTIL_GESTUREARRAY_H_

#include "Board.h"

enum gestures { NONE, MOVE_UP, SHAKE, DRINK };

struct gestureArray {
    enum gestures* arr;
    enum gestures avg;
    uint8_t i;
    uint8_t size;
};

struct gestureArray* createGestureArray(uint8_t size, enum gestures startGesure);
void freeGestureArray(struct gestureArray* arr);
void updateGestureArray(struct gestureArray *arr, enum gestures gesture);
enum gestures calcGestureArrayAvg(enum gestures* arr, uint8_t size);
bool gestureArrayContains(struct gestureArray* arr, enum gestures gesture);

#endif /* UTIL_GESTUREARRAY_H_ */
