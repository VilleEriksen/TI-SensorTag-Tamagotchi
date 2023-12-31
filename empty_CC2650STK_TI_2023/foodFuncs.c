/*
 * food_funcs.c
 *
 *  Created on: 2 Nov 2023
 *      Author: Ville
 */

#include <foodFuncs.h>
#include <xdc/runtime/System.h>
#include "communication.h"

#define SALMIAKKI 3
#define RUISLEIPA 1
#define KORVAPUUSTI 2
#define LOHIKEITTO 5
#define LANTTUKUKKO 6
#define ROSSYPOTTU 4

#define SALMIAKKI_HAPPINESS 6
#define RUISLEIPA_HAPPINESS 2
#define KORVAPUUSTI_HAPPINESS 9
#define LOHIKEITTO_HAPPINESS 5
#define LANTTUKUKKO_HAPPINESS 7
#define ROSSYPOTTU_HAPPINESS -2

#define SALMIAKKI_COST 2
#define RUISLEIPA_COST 1
#define KORVAPUUSTI_COST 1
#define LOHIKEITTO_COST 4
#define LANTTUKUKKO_COST 3
#define ROSSYPOTTU_COST 1

extern int coins;

void feedSalmiakki() {
    if(coins >= SALMIAKKI_COST) {
        giveFood(SALMIAKKI);
        pet(SALMIAKKI_HAPPINESS);
        updateCoins(SALMIAKKI_COST);
    } else {
        sendString("MSG1:NO_MONEY");
    }
}

void feedRuisleipa() {
    if(coins >= RUISLEIPA_COST) {
        giveFood(RUISLEIPA);
        pet(RUISLEIPA_HAPPINESS);
        updateCoins(RUISLEIPA_COST);
    } else {
        sendString("MSG1:NO_MONEY");
    }
}

void feedKorvapuusti() {
    if(coins >= KORVAPUUSTI_COST) {
        giveFood(KORVAPUUSTI);
        pet(KORVAPUUSTI_HAPPINESS);
        updateCoins(KORVAPUUSTI_COST);
    } else {
        sendString("MSG1:NO_MONEY");
    }
}

void feedLohikeitto() {
    if(coins >= LOHIKEITTO_COST) {
        giveFood(LOHIKEITTO);
        pet(LOHIKEITTO_HAPPINESS);
        updateCoins(LOHIKEITTO_COST);
    } else {
        sendString("MSG1:NO_MONEY");
    }
}

void feedLanttukukko() {
    if(coins >= LANTTUKUKKO_COST) {
        giveFood(LANTTUKUKKO);
        pet(LANTTUKUKKO_HAPPINESS);
        updateCoins(LANTTUKUKKO_COST);
    } else {
        sendString("MSG1:NO_MONEY");
    }

}

void feedRossypottu() {
    if(coins >= ROSSYPOTTU_COST) {
        giveFood(ROSSYPOTTU);
        pet(ROSSYPOTTU_HAPPINESS);
        updateCoins(ROSSYPOTTU_COST);
    } else {
        sendString("MSG1:NO_MONEY");
    }
}
