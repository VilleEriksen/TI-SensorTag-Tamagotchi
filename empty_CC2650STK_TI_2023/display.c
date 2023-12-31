/*
 * display.c
 *
 *  Created on: 31 Oct 2023
 *      Author: Ville
 */

#include <string.h>
#include <stdio.h>

#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/mw/display/Display.h>
#include <ti/mw/display/DisplayExt.h>

#include "Board.h"
#include "util/textMenu.h"
#include "menu.h"
#include "game.h"
#include "display.h"

extern struct publicGameParams gameParams;

#define DISPLAY_STACK_SIZE 256
char displayStack[DISPLAY_STACK_SIZE];

enum displayMode currentDisplayMode = MENU;

Display_Handle hDisplayLcd;

char msgText[17];

bool displayUpdating = false;
bool updateDisplay = true;
bool partialUpdateDisplay = false;

// Updates the display according to the display mode.
void displayTask() {
   // Modified from Lovelace display code

   // Initialize display
   Display_Params params;
   Display_Params_init(&params);
   params.lineClearMode = DISPLAY_CLEAR_BOTH;

   hDisplayLcd = Display_open(Display_Type_LCD, &params);
   if (!hDisplayLcd) System_abort("Cannot open display");

   tContext *pContext = DisplayExt_getGrlibContext(hDisplayLcd);
   if (!pContext) System_abort("Cannot get display gr context");

   // This is this is the string of the line printed.
   char lineText[17];
   int i;

   while(1) {
       // Check if the display should be updated.
       if (updateDisplay || partialUpdateDisplay) {
           displayUpdating = true;

           switch (currentDisplayMode) {
               case MENU:
                   if (updateDisplay) Display_clear(hDisplayLcd);

                   // Iterate though the menu item.
                   for (i = 0; i < currentMenu->size; i++) {
                       // Format the menu line string.
                       if (i == currentMenu->selectedI) strcpy(lineText, SELECTED_ITEM);
                       else strcpy(lineText, NORMAL_ITEM);

                       strcpy(lineText + 2, (currentMenu->menuItems + i)->itemText);

                       // Finally print the menu line.
                       Display_print0(hDisplayLcd, i, 0, lineText);
                   }

                   break;

               case MESSANGE:
                   Display_clear(hDisplayLcd);
                   // Display the messange text in center
                   Display_print0(hDisplayLcd, 5, 8 - (strlen(msgText) / 2), msgText);

                   break;

               case GAME:
                   Display_clear(hDisplayLcd);
                   // Pipes
                   GrLineDrawV(pContext, gameParams.pipes1XPos, 0, gameParams.pipes1YPos - gameParams.pipesDistance);
                   GrLineDrawV(pContext, gameParams.pipes1XPos, SCREEN_FULL_DIM, gameParams.pipes1YPos + gameParams.pipesDistance);

                   GrLineDrawV(pContext, gameParams.pipes2XPos, 0, gameParams.pipes2YPos - gameParams.pipesDistance);
                   GrLineDrawV(pContext, gameParams.pipes2XPos, SCREEN_FULL_DIM, gameParams.pipes2YPos + gameParams.pipesDistance);

                   // Player
                   GrCircleFill(pContext, PLAYER_X_POS, gameParams.playerYpos, 3);
                   GrFlush(pContext);

                   break;
           }

           // After we have updated, set the update flag to false.
           updateDisplay = false;
           partialUpdateDisplay = false;

           displayUpdating = false;
       }

       // Refresh rate
       if (currentDisplayMode == GAME) Task_sleep(33333/Clock_tickPeriod); // 30hz
       else Task_sleep(100000/Clock_tickPeriod); // 10hz

   }
}

// Inits the display task
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
