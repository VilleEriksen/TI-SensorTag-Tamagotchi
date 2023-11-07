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
#include "Board.h"
#include "util/avgArray.h"
#include "empty.h"

extern enum state1 programState;

extern struct avgArray* axAvg;
extern struct avgArray* ayAvg;
extern struct avgArray* azAvg;
extern struct avgArray* gxAvg;
extern struct avgArray* gyAvg;
extern struct avgArray* gzAvg;

extern float ax, ay, az, gx, gy, gz;

#define STACKSIZE 1024
Char taskStack[STACKSIZE];

enum gestures currentGesture = NONE;

void detectGestureFxn(UArg arg0, UArg arg1) {
    float shake;
    char printString[20];

    while(1) {
        if (programState == GYRO_DATA_READY) {
            shake = 0;

            shake += gxAvg->avg - gx;
            shake += gyAvg->avg - gy;
            shake += gzAvg->avg - gz;

            if (shake > SHAKING_THRESHOLD || shake < -SHAKING_THRESHOLD) {
                currentGesture = SHAKE;

                System_printf("Shaking\n");
                System_flush();
            } else if (ayAvg->avg > DRINK_A_TRESHOLD && gyAvg->avg > DRINK_G_TRESHOLD && gyAvg->avg > abs(gxAvg->avg) && gyAvg->avg > abs(gzAvg->avg)) {
                currentGesture = DRINK;

                System_printf("Drinking\n");
                System_flush();
            } else if (gyAvg->avg > MOVING_UP_THRESHOLD && gyAvg->avg > abs(gxAvg->avg) && gyAvg->avg > abs(gzAvg->avg) && abs(axAvg->avg) < MOVING_UP_AX_THRESHOLD && abs(ayAvg->avg) < MOVING_UP_AY_THRESHOLD) {
                currentGesture = MOVE_UP;

                System_printf("Moving up\n");
                System_flush();
            } else {
                currentGesture = NONE;
            }
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
    task = Task_create((Task_FuncPtr)detectGestureFxn, &taskParams, NULL);
    if (task == NULL) {
        System_abort("Task create failed!");
    }
}
