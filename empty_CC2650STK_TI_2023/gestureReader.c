/*
 * gestureReader.c
 *
 *  Created on: 7 Nov 2023
 *      Author: Ville
 */

#include <main.h>
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>

#include <stdio.h>
#include "gestureReader.h"
#include "util/avgArray.h"
#include <util/gestureArray.h>
#include "lightSensor.h"

extern enum state1 programState;

extern bool ligthInitalized;
extern bool gyroInitalized;

extern struct avgArray* axAvg;
extern struct avgArray* ayAvg;
extern struct avgArray* azAvg;
extern struct avgArray* gxAvg;
extern struct avgArray* gyAvg;
extern struct avgArray* gzAvg;

extern struct avgArray* luxAvg;
extern float lux;

extern float ax, ay, az, gx, gy, gz;

struct gestureArray* gestureAvg;

char printString[16];

#define STACKSIZE 16
Char taskStack[STACKSIZE];

enum gestures currentGesture = NONE;

void detectGestureFxn(UArg arg0, UArg arg1) {
    float shake;

    while(1) {
        if (programState == DATA_READY) {
            programState = DETECT_GESTURE;

            shake = 0;

            // Calculate shake by looking at the difference in current gyro vs averages
            shake += abs(gyAvg->avg - gy);
            shake += abs(gzAvg->avg - gz);

            // Try to detect the current gesture, and add that to the gesture average array
            if (ligthInitalized) {
                if(lux == 0) {
                    updateGestureArray(gestureAvg, PET);
                    currentGesture = PET;
                } else {
                    updateGestureArray(gestureAvg, NONE);
                    currentGesture = NONE;
                }
            }  if (gyroInitalized) {
                if (shake > SHAKING_THRESHOLD) {
                    updateGestureArray(gestureAvg, SHAKE);
                    currentGesture = SHAKE;
                } else if (ayAvg->avg > DRINK_A_TRESHOLD && gyAvg->avg > DRINK_G_TRESHOLD && gyAvg->avg > abs(gxAvg->avg) && gyAvg->avg > abs(gzAvg->avg)) {
                    updateGestureArray(gestureAvg, DRINK);
                    currentGesture = DRINK;
                } else if (gyAvg->avg > MOVING_UP_THRESHOLD && gyAvg->avg > abs(gxAvg->avg) && gyAvg->avg > abs(gzAvg->avg) && abs(axAvg->avg) < MOVING_UP_AX_THRESHOLD && abs(ayAvg->avg) < MOVING_UP_AY_THRESHOLD) {
                    updateGestureArray(gestureAvg, MOVE_UP);
                    currentGesture = MOVE_UP;
                } else if (abs(gzAvg->avg) > 40) {
                    updateGestureArray(gestureAvg, PLAY);
                    currentGesture = PLAY;
                } else {
                    updateGestureArray(gestureAvg, NONE);
                    currentGesture = NONE;
                }
            } else {
                currentGesture = NONE;
            }

            programState = GESTURE_READY;
        }

        // Sleep 250ms
        Task_sleep(250000 / Clock_tickPeriod);
    }
}

void initGestureReader() {
    Task_Handle task;
    Task_Params taskParams;

    Task_Params_init(&taskParams);
    taskParams.stackSize = STACKSIZE;
    taskParams.stack = &taskStack;
    taskParams.priority = 2;
    task = Task_create((Task_FuncPtr)detectGestureFxn, &taskParams, NULL);
    if (task == NULL) {
        System_abort("Task create failed!");
    }

    gestureAvg = createGestureArray(5, NONE);
}
