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

bool updateDisplay = false;

void displayTask(UArg arg0, UArg arg1) {

   // Initialize display
   Display_Params params;
   Display_Params_init(&params);
   params.lineClearMode = DISPLAY_CLEAR_BOTH;

   Display_Handle hDisplayLcd = Display_open(Display_Type_LCD, &params);

   if (!hDisplayLcd) System_abort("Cannot open display");

   char lineText[17];

   while(1) {
       if (updateDisplay) {
           Display_clear(hDisplayLcd);

           int i;
           for (i = 0; i < currentMenu->size; i++) {
               if (i == currentMenu->selectedI) strcpy(lineText, SELECTED_ITEM);
               else strcpy(lineText, NORMAL_ITEM);

               strcpy(lineText + 2, (currentMenu->menuItems + i)->itemText);
               Display_print0(hDisplayLcd, i, 0, lineText);
           }

           updateDisplay = false;
       }

       // Refresh rate of 10hz
       Task_sleep(100000/Clock_tickPeriod);
   }
}
