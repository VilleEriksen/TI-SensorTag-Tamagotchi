/*
 * game.h
 *
 *  Created on: 11 Nov 2023
 *      Author: Ville
 */

#ifndef GAME_H_
#define GAME_H_

#include <ti/sysbios/knl/Task.h>

#define SCREEN_FULL_DIM 96
#define SCREEN_HALF_DIM 48;

#define GAME_SPEED_START 33333
#define GAME_SPEED_MULT 0.9995

#define PLAYER_X_POS 15

#define PLAYER_MIN_BOUND 5
#define PLAYER_MAX_BOUND 94


#define PIPES_DIST_START 17
#define PIPES_DIST_END 5

#define GYRO_MULT 0.66
#define PLAYER_DRAG 0.97

struct publicGameParams {
    int pipes1XPos;
    int pipes1YPos;

    int pipes2XPos;
    int pipes2YPos;

    int pipesDistance;

    int playerYpos;

    int score;
};

void gameFxn();
void startGame();
void initGame();

#endif /* GAME_H_ */
