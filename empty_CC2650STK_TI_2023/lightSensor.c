/*
 * lightsensor.c
 *
 *  Created on: 9.11.2023
 *      Author: Joel
 */

/* C Standard library */
#include <stdio.h>
#include <string.h>

/* XDCtools files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC26XX.h>
#include <ti/drivers/UART.h>


/* Board Header files */
#include "Board.h"
#include "sensors/opt3001.h"
#include "empty.h"
#include "display.h"
#include "util/avgArray.h"

#define LIGHT_STACKSIZE 1024
Char lightTaskStack[LIGHT_STACKSIZE];

bool ligthInitalized = false;

extern enum state1 programState;
extern bool i2C_INUSE;

extern enum displayMode currentDisplayMode;
extern bool updateDisplay;
extern char msgText[17];

double ambientLight = -1000.0;

double lux;
char luxString[12];

struct avgArray* luxAvg;

void lightTaskFXn(UArg arg0, UArg arg1) {

    while (programState == GYRO_INIT) {
        Task_sleep(250000 / Clock_tickPeriod);
    }

    currentDisplayMode = MESSANGE;
    strcpy(msgText, "Initializing OPT");
    updateDisplay = true;

    I2C_Handle      i2cOPT;
    I2C_Params      i2cOPTParams;
    I2C_Params_init(&i2cOPTParams);

    i2cOPTParams.bitRate = I2C_400kHz;
    i2cOPT = I2C_open(Board_I2C_TMP, &i2cOPTParams);
       if (i2cOPT == NULL) {
          System_abort("Error Initializing I2C\n");
       }

    System_printf("OPT3001: Setup and calibration...\n");
    System_flush();
    Task_sleep(100000 / Clock_tickPeriod);
    opt3001_setup(&i2cOPT);
    System_printf("OPT3001: Setup and calibration OK\n");
    System_flush();

    currentDisplayMode = MENU;
    updateDisplay = true;

    ligthInitalized = true;

    programState = WAITING;
    while (1) {
        programState = OPT_READ;

        lux = opt3001_get_data(&i2cOPT);
        updateAvgArray(luxAvg, lux);

        programState = DATA_READY;

        Task_sleep(1000000 / Clock_tickPeriod);
    }
}


void initOPT3001() {
    Task_Handle lightTaskHandle;
    Task_Params lightTaskParams;

    /* Task */
    Task_Params_init(&lightTaskParams);
    lightTaskParams.stackSize = LIGHT_STACKSIZE;
    lightTaskParams.stack = &lightTaskStack;
    lightTaskParams.priority=2;
    lightTaskHandle = Task_create(lightTaskFXn, &lightTaskParams, NULL);
    if (lightTaskHandle == NULL) {
        System_abort("Task create failed!");
    }

    luxAvg = createAvgArray(3, 0);
}

