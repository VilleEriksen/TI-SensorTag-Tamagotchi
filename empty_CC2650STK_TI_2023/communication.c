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

void pet(uint8_t petAmount) {
   sprintf(payload, "PET:%d", 1);
   System_printf(payload);
   System_printf("\n");
   System_flush();
   Send6LoWPAN(IEEE80154_SERVER_ADDR, payload, strlen(payload));
   StartReceive6LoWPAN();
}

void exercise(uint8_t exerciseAmount) {
   sprintf(payload, "EXERCISE:%d", 1);
   System_printf(payload);
   System_printf("\n");
   System_flush();
   Send6LoWPAN(IEEE80154_SERVER_ADDR, payload, strlen(payload));
   StartReceive6LoWPAN();
}

void updateCoins(int8_t coinsAmount) {
    coins -= coinsAmount;
    sprintf(payload, "MSG1:Coins:%d", coins);
    System_printf(payload);
    System_printf("\n");
    System_flush();
    Send6LoWPAN(IEEE80154_SERVER_ADDR, payload, strlen(payload));
    StartReceive6LoWPAN();
}

void adjustHappiness(int8_t happinessAmount) {
    happiness += happinessAmount;
    sprintf(payload, "MSG1:Happiness:%d", happiness);
    System_printf(payload);
    System_printf("\n");
    System_flush();
    Send6LoWPAN(IEEE80154_SERVER_ADDR, payload, strlen(payload));
    StartReceive6LoWPAN();
}

void giveFood(uint8_t foodAmount) {
   sprintf(payload, "EAT:%d", foodAmount);
   System_printf(payload);
   System_printf("\n");
   System_flush();
   Send6LoWPAN(IEEE80154_SERVER_ADDR, payload, strlen(payload));
   StartReceive6LoWPAN();
}

void sendString(char string[16]) {

   System_printf(string);
   System_printf("\n");
   System_flush();
   Send6LoWPAN(IEEE80154_SERVER_ADDR, string, strlen(string));
   StartReceive6LoWPAN();
}

void commTaskFxn(UArg arg0, UArg arg1) {

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



