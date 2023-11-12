/*
 * checkBeep.c
 *
 *  Created on: 9.11.2023
 *      Author: Joel
 */

#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>

#include <xdc/runtime/System.h>
#include <xdc/std.h>

#include <stdbool.h>

#include "music.h"


extern bool beepMessage;
#define BEEP_STACKSIZE 32
Char beepTaskStack[BEEP_STACKSIZE];


void beepFxn(UArg arg0, UArg arg1) {
   while(1) {
       if(beepMessage) {
           playWaningBeep();
           beepMessage = false;
       }

       Task_sleep(1000000/Clock_tickPeriod);
   }

}

void initBeep() {
     Task_Handle task;
     Task_Params taskParams;
     Task_Params_init(&taskParams);
     taskParams.stackSize = BEEP_STACKSIZE;
     taskParams.stack = &beepTaskStack;
     task = Task_create((Task_FuncPtr)beepFxn, &taskParams, NULL);
     if (task == NULL) {
         System_abort("Task create failed!");
     }
}

