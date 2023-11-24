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

// Move up in menu
void menuUp() {
    if (currentDisplayMode == MENU && !displayUpdating) {
        menu_moveBy(-1);
        partialUpdateDisplay = true;
    }
}

// Move down in menu
void menuDown() {
    if (currentDisplayMode == MENU && !displayUpdating) {
        menu_moveBy(1);
        partialUpdateDisplay = true;
    }
}

// Activate selected menu item
void menuActivate() {
    if (currentDisplayMode == MENU && !displayUpdating) {
        menu_activate(1);
        updateDisplay = true;
    }
}
