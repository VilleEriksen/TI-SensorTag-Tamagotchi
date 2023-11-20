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
#include "gyro.h"
#include "lightSensor.h"
#include "menu.h"

#include "Board.h"

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
    { .itemText = "Salmiakki   2e", .hasFunc = true, .nextFunc = feedSalmiakki },
    { .itemText = "Ruisleipa   1e", .hasFunc = true, .nextFunc = feedRuisleipa },
    { .itemText = "Korvapuusti 1e", .hasFunc = true, .nextFunc = feedKorvapuusti },
    { .itemText = "Lohikeitto  4e", .hasFunc = true, .nextFunc = feedLohikeitto },
    { .itemText = "Lanttukukko 3e", .hasFunc = true,     .nextFunc = feedLanttukukko },
    { .itemText = "Rossypottu  1e", .hasFunc = true, .nextFunc = feedRossypottu },
    { .itemText = "Back",           .hasFunc = true, .nextFunc = menu_reset },
};

static struct menu FOOD_MENU = { &FOOD_MENU_ITEMS[0], &FOOD_MENU_ITEMS[0], 0, 7 };

// Options menu

static struct menuItem OPTIONS_MENU_ITEMS[] = {
    { .itemText = "Toggle audio", .hasFunc = true, .nextFunc = toggleMusic },
    { .itemText = "Death off",    .hasFunc = true },
    { .itemText = "Start new",    .hasFunc = true },
    { .itemText = "Init MPU",     .hasFunc = true,  .nextFunc = initMPU920 },
    { .itemText = "Init OPT",     .hasFunc = true,  .nextFunc = initOPT3001 },
    { .itemText = "Back",         .hasFunc = true, .nextFunc = menu_reset },
};

static struct menu OPTIONS_MENU = { &OPTIONS_MENU_ITEMS[0], &OPTIONS_MENU_ITEMS[0], 0, 6 };

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
