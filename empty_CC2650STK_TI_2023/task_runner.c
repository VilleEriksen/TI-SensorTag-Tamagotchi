/*
 * task_runner.c
 *
 *  Created on: 2 Nov 2023
 *      Author: Ville
 */

#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/knl/Task.h>

#include "display.h"

#include "task_runner.h"

void initTasks() {
    initDisplay();
}

void initDisplay() {
    Task_Params displayTaskParams;
    Task_Handle displayTaskHandle;

    Task_Params_init(&displayTaskParams);
    displayTaskParams.stackSize = DISPLAY_STACK_SIZE;
    displayTaskParams.stack = &displayStack;
    displayTaskParams.priority = 2;

    displayTaskHandle = Task_create((Task_FuncPtr)displayTask, &displayTaskParams, NULL);
    if (displayTaskHandle == NULL) {
        System_abort("Display task create failed");
    }
}
