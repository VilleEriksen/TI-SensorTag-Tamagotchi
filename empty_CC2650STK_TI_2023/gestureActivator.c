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
#include "music.h"
#include "game.h"
#include "communication.h"

#define STACKSIZE2 1024
Char taskStack2[STACKSIZE2];

extern enum state1 programState;
extern struct gestureArray* gestureAvg;
extern bool gameActive;

void activateGestureFxn(UArg arg0, UArg arg1) {
    while(1) {
        if (!gameActive) {
            if (gestureArrayContains(gestureAvg, MOVE_UP)) {
                // TODO: add func
               playMovingUpSting();
               adjustHappiness(1);
               //System_printf("Moving up");
            } else {
                switch (gestureAvg->avg) {;
                   case NONE:
                       // TODO: add func
                       //System_printf("Nothing");
                       break;
                   case PET:
                       pet(1);
                       break;
                   case SHAKE:
                       // TODO: add func
                       playShakingSting();
                       exercise(1);
                       //System_printf("Shaking");
                       break;
                }
            }
        }

        //System_printf("\n");
        //System_flush();

        // Sleep 500ms
        Task_sleep(500000 / Clock_tickPeriod);
    }
}

void initGestureActivator() {
    Task_Handle task2;
    Task_Params taskParams2;

    Task_Params_init(&taskParams2);
    taskParams2.stackSize = STACKSIZE2;
    taskParams2.stack = &taskStack2;
    taskParams2.priority = 2;
    task2 = Task_create((Task_FuncPtr)activateGestureFxn, &taskParams2, NULL);
    if (task2 == NULL) {
        System_abort("Task create failed!");
    }
}
