/*
 * display.c
 *
 *  Created on: 31 Oct 2023
 *      Author: Ville
 */

#include <string.h>

#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/mw/display/Display.h>

#include "Board.h"
#include "util/textMenu.h"
#include "menu.h"
#include "display.h"

#define DISPLAY_STACK_SIZE 256
char displayStack[DISPLAY_STACK_SIZE];

Display_Handle displayHandle;

bool displayUpdating = false;

bool updateDisplay = true;

void displayTask(UArg arg0, UArg arg1) {

   // Initialize display
   Display_Params params;
   Display_Params_init(&params);
   params.lineClearMode = DISPLAY_CLEAR_BOTH;

   Display_Handle hDisplayLcd = Display_open(Display_Type_LCD, &params);

   if (!hDisplayLcd) System_abort("Cannot open display");

   // This is this is the string of the line printed.
   char lineText[17];

   while(1) {
       // Check if the display should be updated.
       if (updateDisplay) {
           displayUpdating = true;

           Display_clear(hDisplayLcd);

           int i;

           // Iterate though the menu item.
           for (i = 0; i < currentMenu->size; i++) {
               // Format the menu line string.
               if (i == currentMenu->selectedI) strcpy(lineText, SELECTED_ITEM);
               else strcpy(lineText, NORMAL_ITEM);

               strcpy(lineText + 2, (currentMenu->menuItems + i)->itemText);

               // Finally print the menu line.
               Display_print0(hDisplayLcd, i, 0, lineText);
           }

           // After we have updated, set the update flag to false.
           updateDisplay = false;

           displayUpdating = false;
       }

       // Refresh rate of 10hz
       Task_sleep(100000/Clock_tickPeriod);
   }
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
