#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <wireless/comm_lib.h>

#include "Board.h"
#include "music.h"
#include "communication.h"
#include <stdio.h>
#include <string.h>

extern int coins;
extern int happiness;

extern bool musicPlaying;

#define COMMSTACKSIZE 512
char commTaskStack[COMMSTACKSIZE];
char payload[16];

bool beepMessage = false;

// Adjust pet need wirelessly
// petAmount: Amount to adjust
void pet(int8_t petAmount) {
   sprintf(payload, "PET:%d", 1);
   System_printf(payload);
   System_printf("\n");
   System_flush();
   Send6LoWPAN(IEEE80154_SERVER_ADDR, payload, strlen(payload));
   StartReceive6LoWPAN();
}

// Adjust exercise need wirelessly
// exerciseAmount: Amount to adjust
void exercise(uint8_t exerciseAmount) {
   sprintf(payload, "EXERCISE:%d", 1);
   System_printf(payload);
   System_printf("\n");
   System_flush();
   Send6LoWPAN(IEEE80154_SERVER_ADDR, payload, strlen(payload));
   StartReceive6LoWPAN();
}

// Update coins wirelessly
// coinsAmount: Amount to adjust
void updateCoins(int8_t coinsAmount) {
    coins -= coinsAmount;
    sprintf(payload, "MSG1:Coins:%d", coins);
    System_printf(payload);
    System_printf("\n");
    System_flush();
    Send6LoWPAN(IEEE80154_SERVER_ADDR, payload, strlen(payload));
    StartReceive6LoWPAN();
}

// Adjust happiness need wirelessly
// happinessAmount: Amount to adjust
void adjustHappiness(int8_t happinessAmount) {
    happiness += happinessAmount;
    sprintf(payload, "MSG1:Happiness:%d", happiness);
    System_printf(payload);
    System_printf("\n");
    System_flush();
    Send6LoWPAN(IEEE80154_SERVER_ADDR, payload, strlen(payload));
    StartReceive6LoWPAN();
}

// Adjust hunger need wirelessly
// foodAmount: Amount to adjust
void giveFood(uint8_t foodAmount) {
   sprintf(payload, "EAT:%d", foodAmount);
   System_printf(payload);
   System_printf("\n");
   System_flush();
   Send6LoWPAN(IEEE80154_SERVER_ADDR, payload, strlen(payload));
   StartReceive6LoWPAN();
}

// Will send a string wirelessly
// string: The string to send
void sendString(char string[16]) {
   System_printf(string);
   System_printf("\n");
   System_flush();
   Send6LoWPAN(IEEE80154_SERVER_ADDR, string, strlen(string));
   StartReceive6LoWPAN();
}

// Inits the wireless sensor and listens for signals
void commTaskFxn() {

   char payload[16];
   uint16_t senderAddr;
   int32_t result = StartReceive6LoWPAN();
   if(result != true) {
      System_abort("Wireless receive start failed");
   }

   //receiving
   while (true) {
       if (GetRXFlag()) {
           memset(payload,0,16);
           Receive6LoWPAN(&senderAddr, payload, 16);
           // check for beep message
           if(strstr(payload, "3254,BEEP") != NULL) {
               playWaningBeep();
               System_printf(payload);
               System_flush();
           }
       }
   }
}

// Inits the communication task
void initCommunication() {
    Init6LoWPAN();

    Task_Params commTaskParams;
    Task_Handle commTaskHandle;

    Task_Params_init(&commTaskParams);
    commTaskParams.stackSize = COMMSTACKSIZE;
    commTaskParams.stack = &commTaskStack;
    commTaskParams.priority = 1;
    commTaskHandle = Task_create(commTaskFxn, &commTaskParams, NULL);

    if (commTaskHandle == NULL) {
        System_abort("Task create failed!");
    }
}



