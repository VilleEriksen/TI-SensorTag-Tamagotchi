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
#include "util/avgArray.h"

#define LIGHT_STACKSIZE 1024
Char lightTaskStack[LIGHT_STACKSIZE];

extern enum state1 programState;
extern bool i2C_INUSE;

double ambientLight = -1000.0;

double lux;
char luxString[12];

struct avgArray* luxAvg;

void lightTaskFXn(UArg arg0, UArg arg1) {

    while (programState == GYRO_INIT) {
        Task_sleep(250000 / Clock_tickPeriod);
    }

    enum state1 oldState;

    programState = OPT_INIT;

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
    I2C_close(i2cOPT);
    System_printf("OPT3001: Setup and calibration OK\n");
    System_flush();

    programState = WAITING;
    while (1) {
        if(programState != GYRO_READ) {
            oldState = programState;
            programState = OPT_READ;
            i2cOPT = I2C_open(Board_I2C_TMP, &i2cOPTParams);
            Task_sleep(1000000 / Clock_tickPeriod);

            lux = opt3001_get_data(&i2cOPT);
            updateAvgArray(luxAvg, lux);

            if (lux != -1) {
                //System_printf("sensorTask: ");
                //sprintf(luxString,"%.2lf\n",lux);
                //System_printf(luxString);
            }

            if(luxAvg -> avg < 0.10) {
                //System_printf("Petting \n");
            }

            // Just for sanity check for exercise, you can comment this out
            System_flush();
            I2C_close(i2cOPT);
            Task_sleep(1000000 / Clock_tickPeriod);
            // Once per second, you can modify this
            programState = oldState;
            Task_sleep(1000000 / Clock_tickPeriod);

        }
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
