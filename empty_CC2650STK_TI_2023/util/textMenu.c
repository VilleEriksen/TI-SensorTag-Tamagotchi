/*
 * textMenu.c
 *
 *  Created on: 31 Oct 2023
 *      Author: Ville
 */

#include <stdlib.h>
#include "textMenu.h"

// Move menu by steps
// steps: Steps to move
void menu_moveBy(int steps) {
    currentMenu->selectedI = (currentMenu->selectedI + steps) % currentMenu->size;
    currentMenu->selectedItem = currentMenu->menuItems + currentMenu->selectedI;
}

// Activate selected item in menu
void menu_activate() {
    if (currentMenu->selectedItem->hasFunc) {
        currentMenu->selectedItem->nextFunc();
        currentMenu = baseMenu;
    } else {
        currentMenu = currentMenu->selectedItem->nextMenu;
    }
}

// Return to base menu
void menu_reset() {
    currentMenu = baseMenu;
}

// Init menu
void menu_init() {
    baseMenu = &MAIN_MENU;
    currentMenu = baseMenu;
}
