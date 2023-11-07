/*
 * textMenu.c
 *
 *  Created on: 31 Oct 2023
 *      Author: Ville
 */

#include <stdlib.h>
#include "textMenu.h"

void menu_moveBy(int steps) {
    currentMenu->selectedI = (currentMenu->selectedI + steps) % currentMenu->size;
    currentMenu->selectedItem = currentMenu->menuItems + currentMenu->selectedI;
}

void menu_activate() {
    if (currentMenu->selectedItem->hasFunc) {
        currentMenu->selectedItem->nextFunc();
        currentMenu = baseMenu;
    } else {
        currentMenu = currentMenu->selectedItem->nextMenu;
    }
}

void menu_reset() {
    currentMenu = baseMenu;
}

void menu_init() {
    baseMenu = &MAIN_MENU;
    currentMenu = baseMenu;
}
