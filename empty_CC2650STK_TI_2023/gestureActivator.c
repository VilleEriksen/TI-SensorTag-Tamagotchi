/*
 * gestureActivator.c
 *
 *  Created on: 9 Nov 2023
 *      Author: Ville
 */

#include <main.h>
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>

#include <stdio.h>
#include <util/gestureArray.h>
#include "music.h"
#include "game.h"

#include "communication.h"

#define STACKSIZE2 1024
Char taskStack2[STACKSIZE2];

extern enum gestures currentGesture;
extern enum state1 programState;
extern struct gestureArray* gestureAvg;
extern bool gameActive;

// Determines what gestures to activate from the detected gestures
void activateGestureFxn() {
    while(1) {
        if (!gameActive) {
            // Look at the average array, and activate the gestures accordingly

            // The move up gesture is a lot quicker, so we will not look at the average
            if (gestureArrayContains(gestureAvg, MOVE_UP)) {
               playMovingUpSting();
               pet(1);

            } else {
                switch (gestureAvg->avg) {;
                   case NONE:
                       break;
                   case PET:
                       playShakingSting();
                       pet(1);
                       break;
                   case SHAKE:
                       playShakingSting();
                       exercise(1);
                       break;
                   case PLAY:
                       playPlayingString();
                       exercise(1);
                       pet(1);
                       break;
                }
            }
        }

        // Sleep 500ms
        Task_sleep(500000 / Clock_tickPeriod);
    }
}

// Inits the gesture activator task
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
