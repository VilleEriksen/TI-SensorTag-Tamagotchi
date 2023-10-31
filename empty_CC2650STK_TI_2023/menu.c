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

void display_menuUpdate() {
    int i;
    for (i = 0; i < currentMenu->size; i++) {
        if (i == currentMenu->selectedI) System_printf(SELECTED_ITEM);
        else System_printf(NORMAL_ITEM);

        System_printf((currentMenu->menuItems + i)->itemText);
        System_printf("\n");
    }

    System_printf("\n\n\n\n\n\n\n\n\n\n\n");
    System_flush();
}

void display_menuUp() {
    menu_moveBy(-1);
    display_menuUpdate();
}

void display_menuDown() {
    menu_moveBy(1);
    display_menuUpdate();
}

void display_menuActivate() {
    menu_activate(1);
    display_menuUpdate();
}
