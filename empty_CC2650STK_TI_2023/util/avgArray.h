/*
 * avgArray.h
 *
 *  Created on: 30 Oct 2023
 *      Author: Ville
 */

#ifndef UTIL_AVGARRAY_H_
#define UTIL_AVGARRAY_H_

struct avgArray {
    float* arr;
    float avg;
    int i;
    int size;
};

struct avgArray* createAvgArray(int size, float startVal);
void freeAvgArray(struct avgArray* arr);
void updateAvgArray(struct avgArray *arr, float val);
float calcAvg(float* arr, int size);

#endif /* UTIL_AVGARRAY_H_ */
