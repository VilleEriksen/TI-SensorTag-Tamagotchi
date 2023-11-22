/*
 * menu.c
 *
 *  Created on: 31 Oct 2023
 *      Author: Ville
 */
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <string.h>
#include "Board.h"
#include "util/textMenu.h"
#include "menu.h"
#include "display.h"

extern bool displayUpdating;
extern bool updateDisplay;
extern bool partialUpdateDisplay;
extern enum displayMode currentDisplayMode;

void menuUp() {
    if (currentDisplayMode == MENU && !displayUpdating) {
        menu_moveBy(-1);
        partialUpdateDisplay = true;
    }
}

void menuDown() {
    if (currentDisplayMode == MENU && !displayUpdating) {
        menu_moveBy(1);
        partialUpdateDisplay = true;
    }
}

void menuActivate() {
    if (currentDisplayMode == MENU && !displayUpdating) {
        menu_activate(1);
        updateDisplay = true;
    }
}
