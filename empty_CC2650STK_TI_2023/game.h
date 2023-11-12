/*
 * game.h
 *
 *  Created on: 11 Nov 2023
 *      Author: Ville
 */

#ifndef GAME_H_
#define GAME_H_

struct publicGameParams {
    int pipes1XPos;
    int pipes1YPos;

    int pipes2XPos;
    int pipes2YPos;

    int playerYpos;

    int score;
};

void gameFxn(UArg arg0, UArg arg1);
void startGame();
void initGame();

#endif /* GAME_H_ */
