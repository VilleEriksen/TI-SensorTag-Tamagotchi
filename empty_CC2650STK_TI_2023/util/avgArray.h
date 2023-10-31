/*
 * avgArray.h
 *
 *  Created on: 30 Oct 2023
 *      Author: Ville
 */

#ifndef UTIL_AVGARRAY_H_
#define UTIL_AVGARRAY_H_

#include "Board.h"

struct avgArray {
    float* arr;
    float avg;
    uint8_t i;
    uint8_t size;
};

struct avgArray* createAvgArray(uint8_t size, float startVal);
void freeAvgArray(struct avgArray* arr);
void updateAvgArray(struct avgArray *arr, float val);
float calcAvg(float* arr, uint8_t size);

#endif /* UTIL_AVGARRAY_H_ */
