/*
 * communication.h
 *
 *  Created on: Nov 6, 2023
 *      Author: Joel
 */

#include <xdc/std.h>

#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_


void commTaskFxn(UArg arg0, UArg arg1);
void initCommunication();

void giveFood(uint8_t foodAmount);
void pet(uint8_t petAmount);
void exercise(uint8_t exerciseAmount);
void updateCoins(int8_t coinsAmount);
void sendString(char string[16]);



#endif /* COMMUNICATION_H_ */
