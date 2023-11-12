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

enum displayMode { MENU, MESSANGE, GAME };

void displayTask(UArg arg0, UArg arg1);
void initDisplay();

#endif /* DISPLAY_H_ */
