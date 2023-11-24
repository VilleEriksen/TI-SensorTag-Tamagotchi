/*
 * gestureReader.h
 *
 *  Created on: 7 Nov 2023
 *      Author: Ville
 */

#ifndef GESTUREREADER_H_
#define GESTUREREADER_H_

#include <ti/sysbios/knl/Task.h>
#include <util/gestureArray.h>

#define SHAKING_THRESHOLD 100
#define MOVING_UP_THRESHOLD 20
#define MOVING_UP_AX_THRESHOLD 0.5
#define MOVING_UP_AY_THRESHOLD 0.5
#define DRINK_A_TRESHOLD 0.75
#define DRINK_G_TRESHOLD 20

void detectGestureFxn();
void initGestureReader();



#endif /* GESTUREREADER_H_ */
