/*
 * game.c
 *
 *  Created on: 11 Nov 2023
 *      Author: Ville
 */

#include <stdio.h>
#include <string.h>

#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>

#include "Board.h"
#include "gyro.h"
#include "display.h"
#include "communication.h"
#include "game.h"

const uint8_t randArr[] = {
    66, 27, 72, 73, 51, 26, 74, 73, 74, 67,
    51, 50, 50, 39, 57, 29, 41, 63, 69, 44,
    28, 66, 48, 23, 37, 56, 28, 17, 72, 59,
    58, 59, 63, 40, 33, 59, 33, 72, 25, 63,
    17, 21, 50, 71, 18, 40, 50, 63, 21, 19,
    50, 77, 64, 48, 63, 70, 57, 77, 31, 17,
    60, 75, 26, 70, 27, 28, 52, 32, 70, 49,
    73, 42, 65, 50, 20, 49, 72, 38, 78, 58,
    31, 21, 55, 22, 39, 42, 58, 76, 39, 77,
    57, 79, 59, 32, 34, 47, 44, 73, 65, 78
};

extern enum displayMode currentDisplayMode;
extern bool updateDisplay;
extern char msgText[17];
extern float ay;

#define GAME_STACKSIZE 1024
Char gameStack[GAME_STACKSIZE];

bool gameActive = false;
bool startCount = false;
bool gameOver = false;
struct publicGameParams gameParams;

float playerYSpeed = 0.0;
float playerY = 0.0;

int i1 = 0;
int i2 = 1;

void gameFxn(UArg arg0, UArg arg1) {

    while (1) {
        if (gameActive) {
            if (startCount) {
                startCount = false;

                currentDisplayMode = MESSANGE;
                Task_sleep(100000 / Clock_tickPeriod);

                strcpy(msgText, "3");
                updateDisplay = true;
                Task_sleep(1000000 / Clock_tickPeriod);
                strcpy(msgText, "2");
                updateDisplay = true;
                Task_sleep(1000000 / Clock_tickPeriod);
                strcpy(msgText, "1");
                updateDisplay = true;
                Task_sleep(1000000 / Clock_tickPeriod);

                currentDisplayMode = GAME;
            }

            if (gameOver) {
                gameOver = false;
                gameActive = false;

                updateCoins(gameParams.score * -1);

                currentDisplayMode = MESSANGE;
                Task_sleep(100000 / Clock_tickPeriod);

                strcpy(msgText, "Game Over!");
                updateDisplay = true;
                Task_sleep(3000000 / Clock_tickPeriod);
                sprintf(msgText, "Score: %d", gameParams.score);
                //strcpy(msgText, "Score: XX");
                updateDisplay = true;
                Task_sleep(3000000 / Clock_tickPeriod);

                currentDisplayMode = MENU;
                Task_sleep(100000 / Clock_tickPeriod);
                updateDisplay = true;
            }

            // Update player pos
            playerYSpeed += ay / 1.5;
            playerYSpeed *= 0.97;
            playerY += playerYSpeed;
            if (playerY <= 5 || playerY >= 92) playerYSpeed = 0;

            gameParams.playerYpos = playerY;

            // Update pipes pos
            gameParams.pipes1XPos--;
            if (gameParams.pipes1XPos <= 0) {
                gameParams.pipes1XPos = 96;
                gameParams.pipes1YPos = randArr[i1];

                i1 = (i1 + 2) % 100;
            }

            gameParams.pipes2XPos--;
            if (gameParams.pipes2XPos <= 0) {
                gameParams.pipes2XPos = 96;
                gameParams.pipes2YPos = randArr[i2];

                i2 = (i2 + 2) % 100;
            }

            if (gameParams.pipes1XPos == 15) {
                if (gameParams.playerYpos > gameParams.pipes1YPos + 17 || gameParams.playerYpos < gameParams.pipes1YPos - 17) {
                    gameOver = true;
                } else {
                    gameParams.score++;
                }
            }

            if (gameParams.pipes2XPos == 15) {
                if (gameParams.playerYpos > gameParams.pipes2YPos + 17 || gameParams.playerYpos < gameParams.pipes2YPos - 17) {
                    gameOver = true;
                } else {
                    gameParams.score++;
                }
            }

            updateDisplay = true;
        }

        // Refresh rate 10hz
        //Task_sleep(100000 / Clock_tickPeriod);
        Task_sleep(33333/Clock_tickPeriod);
    }
}

void startGame() {
    gameParams.pipes1XPos = 96;
    gameParams.pipes1YPos = 48;
    gameParams.pipes2XPos = 48;
    gameParams.pipes2YPos = 48;
    gameParams.playerYpos = 48;
    gameParams.score = 0;

    playerY = 48;

    startCount = true;
    gameActive = true;
}

void initGame() {
    Task_Handle gameTask;
    Task_Params gameTaskParams;

    Task_Params_init(&gameTaskParams);
    gameTaskParams.stackSize = GAME_STACKSIZE;
    gameTaskParams.stack = &gameStack;
    gameTask = Task_create((Task_FuncPtr)gameFxn, &gameTaskParams, NULL);
    if (gameTask == NULL) {
        System_abort("Game create failed!");
    }
}
