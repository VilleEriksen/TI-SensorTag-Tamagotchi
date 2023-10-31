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

void updateMenu() {
    char printString[16];

    int i;
    for (i = 0; i < currentMenu->size; i++) {
        if (i == currentMenu->selectedI) {
            printString = "> ";
            strcat(printString, currentMenu->selectedItem->itemText);
        } else {
            printString = "  ";
            strcat(printString, currentMenu->selectedItem->itemText);
        }

        System_printf(printString);
    }

    System_flush();
}


