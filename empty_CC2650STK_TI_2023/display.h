/*
 * display.h
 *
 *  Created on: 31 Oct 2023
 *      Author: Ville
 */

#include <xdc/std.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/mw/display/Display.h>

#include "Board.h"

#ifndef DISPLAY_H_
#define DISPLAY_H_

#define DISPLAY_STACK_SIZE 256
char displayStack[DISPLAY_STACK_SIZE];

Display_Handle displayHandle;

void displayTask(UArg arg0, UArg arg1);

#endif /* DISPLAY_H_ */
