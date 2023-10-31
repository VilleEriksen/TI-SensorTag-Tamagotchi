/*
 * textMenu.h
 *
 *  Created on: 31 Oct 2023
 *      Author: Ville
 */

#ifndef UTIL_TEXTMENU_H_
#define UTIL_TEXTMENU_H_

#include "music.h"

struct menuItem {
    char itemText[14];
    bool hasFunc;
    union {
        struct menu* nextMenu;
        void (*nextFunc)();
    };
};

struct menu {
    struct menuItem* menuItems;
    struct menuItem* selectedItem;
    int selectedI;
    int size;
};

static struct menuItem MUSIC_MENU_ITEMS[] = {
    { .itemText = "Happy theme", .hasFunc = true, .nextFunc = playHappyTheme },
    { .itemText = "Angry theme", .hasFunc = true, .nextFunc = playAngryTheme }
};

static struct menu MUSIC_MENU = { &MUSIC_MENU_ITEMS[0], &MUSIC_MENU_ITEMS[0], 0, 2 };

struct menu* baseMenu;
struct menu* currentMenu;

#endif /* UTIL_TEXTMENU_H_ */
