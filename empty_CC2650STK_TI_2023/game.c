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
#include "music.h"
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

float yBias;

float playerYSpeed = 0.0;
float playerY = 0.0;

int i1 = 0;
int i2 = 1;

float gameSpeed = 100000.0;
char frame = 0;

void gameFxn(UArg arg0, UArg arg1) {

    while (1) {
        if (gameActive) {
            if (startCount) {
                startCount = false;

                currentDisplayMode = MESSANGE;
                //Task_sleep(100000 / Clock_tickPeriod);

                strcpy(msgText, "3");
                updateDisplay = true;
                Task_sleep(1000000 / Clock_tickPeriod);
                strcpy(msgText, "2");
                updateDisplay = true;
                Task_sleep(1000000 / Clock_tickPeriod);
                strcpy(msgText, "1");
                updateDisplay = true;
                Task_sleep(1000000 / Clock_tickPeriod);

                playHappyTheme();
                currentDisplayMode = GAME;
            }

            if (gameOver) {
                gameOver = false;
                gameActive = false;

                stopMusic();

                updateCoins(gameParams.score * -1);

                currentDisplayMode = MESSANGE;
                Task_sleep(100000 / Clock_tickPeriod);

                playGameLoseSting();

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

            frame++;
            gameSpeed *= GAME_SPEED_MULT;

            if (frame == 0 && gameParams.pipesDistance > PIPES_DIST_END) gameParams.pipesDistance--;

            // Update player pos
            playerYSpeed += (ay - yBias) * GYRO_MULT;
            playerYSpeed *= PLAYER_DRAG;
            playerY += playerYSpeed;
            if (playerY <= PLAYER_MIN_BOUND || playerY >= PLAYER_MAX_BOUND) playerYSpeed *= -1;

            gameParams.playerYpos = playerY;

            // Update pipes pos
            gameParams.pipes1XPos--;
            if (gameParams.pipes1XPos <= 0) {
                gameParams.pipes1XPos = SCREEN_FULL_DIM;
                gameParams.pipes1YPos = randArr[i1];

                i1 = (i1 + 2) % 100;
            }

            gameParams.pipes2XPos--;
            if (gameParams.pipes2XPos <= 0) {
                gameParams.pipes2XPos = SCREEN_FULL_DIM;
                gameParams.pipes2YPos = randArr[i2];

                i2 = (i2 + 2) % 100;
            }

            if (gameParams.pipes1XPos == PLAYER_X_POS) {
                if (gameParams.playerYpos > gameParams.pipes1YPos + gameParams.pipesDistance || gameParams.playerYpos < gameParams.pipes1YPos - gameParams.pipesDistance) {
                    gameOver = true;
                } else {
                    playPipesEffect();
                    gameParams.score++;
                }
            }

            if (gameParams.pipes2XPos == PLAYER_X_POS) {
                if (gameParams.playerYpos > gameParams.pipes2YPos + gameParams.pipesDistance || gameParams.playerYpos < gameParams.pipes2YPos - gameParams.pipesDistance) {
                    gameOver = true;
                } else {
                    playPipesEffect();
                    gameParams.score++;
                }
            }

            updateDisplay = true;
        }

        // Refresh rate 10hz
        //Task_sleep(100000 / Clock_tickPeriod);
        Task_sleep((int)gameSpeed/Clock_tickPeriod);
    }
}

void startGame() {
    gameParams.pipes1XPos = SCREEN_FULL_DIM;
    gameParams.pipes1YPos = SCREEN_HALF_DIM;
    gameParams.pipes2XPos = SCREEN_HALF_DIM;
    gameParams.pipes2YPos = SCREEN_HALF_DIM;
    gameParams.pipesDistance = PIPES_DIST_START;
    gameParams.playerYpos = SCREEN_HALF_DIM;
    gameParams.score = 0;

    playerY = SCREEN_HALF_DIM;
    yBias = ay;

    gameSpeed = GAME_SPEED_START;

    startCount = true;
    gameActive = true;
}

void initGame() {
    Task_Handle gameTask;
    Task_Params gameTaskParams;

    Task_Params_init(&gameTaskParams);
    gameTaskParams.stackSize = GAME_STACKSIZE;
    gameTaskParams.stack = &gameStack;
    gameTaskParams.priority = 2;
    gameTask = Task_create((Task_FuncPtr)gameFxn, &gameTaskParams, NULL);
    if (gameTask == NULL) {
        System_abort("Game create failed!");
    }
}
