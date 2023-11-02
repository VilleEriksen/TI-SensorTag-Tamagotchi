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

extern bool updateDisplay;

void display_menuUp() {
    menu_moveBy(-1);
    updateDisplay = true;
}

void display_menuDown() {
    menu_moveBy(1);
    updateDisplay = true;
}

void display_menuActivate() {
    menu_activate(1);
    updateDisplay = true;
}
