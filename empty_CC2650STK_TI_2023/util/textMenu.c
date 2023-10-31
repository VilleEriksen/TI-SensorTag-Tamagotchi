/*
 * textMenu.c
 *
 *  Created on: 31 Oct 2023
 *      Author: Ville
 */

#include <stdlib.h>
#include "textMenu.h"

void menu_moveBy(int steps) {
    currentMenu->selectedI = abs(currentMenu->selectedI + steps) % currentMenu->size;
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

void menu_init() {
    baseMenu = &MUSIC_MENU;
    currentMenu = baseMenu;
}

/*
struct textMenu* createSubMenu(struct menuItem* menuItems, int size) {
    struct subMenu* menu = (struct subMenu*)malloc(sizeof(struct subMenu));

    menu->menuItems = menuItems;
    menu->selectedItem = *menuItems;
    menu->selectedI = 0;
    menu->size = size;
}
*/



