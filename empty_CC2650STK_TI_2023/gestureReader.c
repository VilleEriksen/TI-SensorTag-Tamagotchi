/*
 * gestureReader.c
 *
 *  Created on: 7 Nov 2023
 *      Author: Ville
 */

#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>

#include <stdio.h>
#include "gestureReader.h"
#include "util/avgArray.h"
#include <util/gestureArray.h>
#include "empty.h"

extern enum state1 programState;

extern struct avgArray* axAvg;
extern struct avgArray* ayAvg;
extern struct avgArray* azAvg;
extern struct avgArray* gxAvg;
extern struct avgArray* gyAvg;
extern struct avgArray* gzAvg;

extern float ax, ay, az, gx, gy, gz;

struct gestureArray* gestureAvg;

#define STACKSIZE 16
Char taskStack[STACKSIZE];

enum gestures currentGesture = NONE;

void detectGestureFxn(UArg arg0, UArg arg1) {
    float shake;

    while(1) {
        if (programState == GYRO_DATA_READY) {
            programState = DETECT_GESTURE;

            shake = 0;

            //shake += abs(gxAvg->avg - gx);
            shake += abs(gyAvg->avg - gy);
            shake += abs(gzAvg->avg - gz);

            if (shake > SHAKING_THRESHOLD) {
                updateGestureArray(gestureAvg, SHAKE);
                currentGesture = SHAKE;
            } else if (ayAvg->avg > DRINK_A_TRESHOLD && gyAvg->avg > DRINK_G_TRESHOLD && gyAvg->avg > abs(gxAvg->avg) && gyAvg->avg > abs(gzAvg->avg)) {
                updateGestureArray(gestureAvg, DRINK);
                currentGesture = DRINK;
            } else if (gyAvg->avg > MOVING_UP_THRESHOLD && gyAvg->avg > abs(gxAvg->avg) && gyAvg->avg > abs(gzAvg->avg) && abs(axAvg->avg) < MOVING_UP_AX_THRESHOLD && abs(ayAvg->avg) < MOVING_UP_AY_THRESHOLD) {
                updateGestureArray(gestureAvg, MOVE_UP);
                currentGesture = MOVE_UP;
            } else {
                updateGestureArray(gestureAvg, NONE);
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
