/*
 * gestureActivator.c
 *
 *  Created on: 9 Nov 2023
 *      Author: Ville
 */

#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>

#include <stdio.h>
#include <util/gestureArray.h>
#include "empty.h"

#define STACKSIZE2 512
Char taskStack2[STACKSIZE2];

extern enum state1 programState;
extern struct gestureArray* gestureAvg;

void activateGestureFxn(UArg arg0, UArg arg1) {
    while(1) {
        //if (programState == GESTURE_READY) {
        //    programState = ACTIVATING_GESTURE;

            switch (gestureAvg->avg) {;
                case NONE:
                    // TODO: add func
                    System_printf("Nothing");
                    break;
                case MOVE_UP:
                    // TODO: add func
                    System_printf("Moving up");
                    break;
                case SHAKE:
                    // TODO: add func
                    System_printf("Shaking");
                    break;
                case DRINK:
                    // TODO: add func
                    System_printf("Drinking");
                    break;
            }

            System_printf("\n");
            System_flush();

        //    programState = WAITING;
        //}

        // Sleep 1000ms
        Task_sleep(1000000 / Clock_tickPeriod);
    }
}

void initGestureActivator() {
    Task_Handle task2;
    Task_Params taskParams2;

    Task_Params_init(&taskParams2);
    taskParams2.stackSize = STACKSIZE2;
    taskParams2.stack = &taskStack2;
    task2 = Task_create((Task_FuncPtr)activateGestureFxn, &taskParams2, NULL);
    if (task2 == NULL) {
        System_abort("Task create failed!");
    }
}
