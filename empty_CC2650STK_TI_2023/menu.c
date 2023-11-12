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

void display_menuUp() {
    if (!displayUpdating) {
        menu_moveBy(-1);
        partialUpdateDisplay = true;
    }
}

void display_menuDown() {
    if (!displayUpdating) {
        menu_moveBy(1);
        partialUpdateDisplay = true;
    }
}

void display_menuActivate() {
    if (!displayUpdating) {
        menu_activate(1);
        updateDisplay = true;
    }
}
