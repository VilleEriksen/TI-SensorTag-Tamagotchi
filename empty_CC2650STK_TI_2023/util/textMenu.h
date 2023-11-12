/*
 * textMenu.h
 *
 *  Created on: 31 Oct 2023
 *      Author: Ville
 */

#ifndef UTIL_TEXTMENU_H_
#define UTIL_TEXTMENU_H_

#include "music.h"
#include "food_funcs.h"
#include "game.h"
#include "menu.h"

void menu_moveBy(int steps);
void menu_activate();
void menu_reset();
void menu_init();

struct menuItem {
    char itemText[15];
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

// Music menu
static struct menuItem MUSIC_MENU_ITEMS[] = {
    { .itemText = "Happy theme",    .hasFunc = true, .nextFunc = playHappyTheme },
    { .itemText = "Angry theme",    .hasFunc = true, .nextFunc = playAngryTheme },
    { .itemText = "Warning beep",   .hasFunc = true, .nextFunc = playWaningBeep },
    { .itemText = "Moving up beep", .hasFunc = true, .nextFunc = playMovingUpSting },
    { .itemText = "Shaking beep",   .hasFunc = true, .nextFunc = playShakingSting },
    { .itemText = "Back",           .hasFunc = true, .nextFunc = menu_reset },
};

static struct menu MUSIC_MENU = { &MUSIC_MENU_ITEMS[0], &MUSIC_MENU_ITEMS[0], 0, 6 };

// Food menu

static struct menuItem FOOD_MENU_ITEMS[] = {
    { .itemText = "Salmiakki",      .hasFunc = true, .nextFunc = feedSalmiakki },
    { .itemText = "Ruisleipa",      .hasFunc = true, .nextFunc = feedRuisleipa },
    { .itemText = "Korvapuusti",    .hasFunc = true, .nextFunc = feedKorvapuusti },
    { .itemText = "Lohikeitto",     .hasFunc = true, .nextFunc = feedLohikeitto },
    { .itemText = "Karjalanpaisti", .hasFunc = true, .nextFunc = feedKarjalanpaisti },
    { .itemText = "Rossypottu",     .hasFunc = true, .nextFunc = feedRossypottu },
    { .itemText = "Back",           .hasFunc = true, .nextFunc = menu_reset },
};

static struct menu FOOD_MENU = { &FOOD_MENU_ITEMS[0], &FOOD_MENU_ITEMS[0], 0, 7 };

// Options menu

static struct menuItem OPTIONS_MENU_ITEMS[] = {
    { .itemText = "Toggle audio", .hasFunc = true, .nextFunc = toggleMusic },
    { .itemText = "Death off",    .hasFunc = true },
    { .itemText = "Start new",    .hasFunc = true },
    { .itemText = "Back",         .hasFunc = true, .nextFunc = menu_reset },
};

static struct menu OPTIONS_MENU = { &OPTIONS_MENU_ITEMS[0], &OPTIONS_MENU_ITEMS[0], 0, 4 };

// Main menu

static struct menuItem MAIN_MENU_ITEMS[] = {
    { .itemText = "Play music", .hasFunc = false, .nextMenu = &MUSIC_MENU },
    { .itemText = "Give food",  .hasFunc = false, .nextMenu = &FOOD_MENU },
    { .itemText = "Play game",  .hasFunc = true,  .nextFunc = startGame },
    { .itemText = "Options",    .hasFunc = false, .nextMenu = &OPTIONS_MENU },
};

static struct menu MAIN_MENU = { &MAIN_MENU_ITEMS[0], &MAIN_MENU_ITEMS[0], 0, 4 };

struct menu* baseMenu;
struct menu* currentMenu;

#endif /* UTIL_TEXTMENU_H_ */
