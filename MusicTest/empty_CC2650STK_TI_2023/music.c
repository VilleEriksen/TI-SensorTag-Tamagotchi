/* XDCtools files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>

/* Board Header files */
#include "Board.h"
#include "buzzer.h"

static Clock_Handle musicHandle;
static Clock_Params musicParams;

static PIN_Handle hBuzzer;
static PIN_State sBuzzer;
PIN_Config cBuzzer[] = {
    Board_BUZZER | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    PIN_TERMINATE
};

//static int a[] = {1047, 0, 1047, 0, 523, 0, 784, 0, 784, 831, 784, 784, 392, 0, 932, 1109, 1047, 0, 1047, 415, 392, 0, 0, 1245, 1397, 0, 1397, 523, 1109, 1397, 1109, 1109};
static int a[] = {1319, 523, 1319, 523, 1319, 523, 1397, 0, 1175, 392, 1175, 392, 1175, 392, 1319, 0, 1046, 349, 1046, 349, 1175, 392, 1046, 523, 784};
int *song =  &a[0];

int songI;
//int songLength = 32;
int songLength = 25;

char buzzerOpened;

void musixFxn(UArg arg0) {
    if (songI >= songLength) {
        // Clock_delete(&musicHandle);

        song =  &a[0];
        songI = 0;
        //return;
    }

    /*
    if (*song == -1 && buzzerOpened) {
        buzzerClose();
        buzzerOpened = 0;

        return;
    }

    if (!buzzerOpened) {
        buzzerOpen(hBuzzer);
        buzzerOpened = 1;
    }
    */



    buzzerSetFrequency(*song);
    song++;
    songI++;
}

void initMusic() {
    // Buzzer
    hBuzzer = PIN_open(&sBuzzer, cBuzzer);
    if (hBuzzer == NULL) {
        System_abort("Pin open failed!");
    }

    Clock_Params_init(&musicParams);
    musicParams.period = 125000 / Clock_tickPeriod;
    musicParams.startFlag = TRUE;
}

void startMusic() {
    songI = 0;
    buzzerOpened = 0;

    buzzerOpen(hBuzzer);

    musicHandle = Clock_create((Clock_FuncPtr)musixFxn, 250000 / Clock_tickPeriod, &musicParams, NULL);
    if (musicHandle == NULL) {
        System_abort("Music create failed");
    }
}
