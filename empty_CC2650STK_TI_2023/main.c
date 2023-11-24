/* XDCtools Header files */
#include <main.h>
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>


/* TI-RTOS Header files */
#include <ti/drivers/PIN.h>
#include <ti/drivers/I2C.h>

/* Board Header files */
#include "Board.h"
#include "menu.h"
#include "display.h"
#include "util/textMenu.h"
#include "gyro.h"
#include "gestureReader.h"
#include "gestureActivator.h"
#include "game.h"
#include "communication.h"
#include "lightSensor.h"

#define BUTTON_0_ID 0
#define BUTTON_1_ID 4

int coins = 50;
int happiness = 50;

static PIN_Handle buttonHandle0;
static PIN_State buttonState0;

PIN_Config buttonConfig0[] = {
   Board_BUTTON0  | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_NEGEDGE,
   PIN_TERMINATE
};


static PIN_Handle buttonHandle1;
static PIN_State buttonState1;

PIN_Config buttonConfig1[] = {
   Board_BUTTON1  | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_NEGEDGE,
   PIN_TERMINATE
};

//global state variables
enum state1 programState = WAITING;

// Control the menu with buttons
void buttonFxn(PIN_Handle handle, PIN_Id pinId) {
    switch (pinId) {
        case BUTTON_0_ID:
            menuActivate();
            break;
        case BUTTON_1_ID:
            menuDown();
            break;
    }
}

/*
 *  ======== main ========
 */
int main(void)
{
    /* Call board init functions */
    Board_initGeneral();
    Board_initI2C();

    // Initalize all tasks and devices.
    initGame();
    initCommunication();
    menu_init();
    initDisplay();
    initGestureReader();
    initGestureActivator();

    // Initalize buttons for the menu
    buttonHandle0 = PIN_open(&buttonState0, buttonConfig0);
    if(!buttonHandle0) {
       System_abort("Error initializing button 0 pin\n");
    }

    if (PIN_registerIntCb(buttonHandle0, &buttonFxn) != 0) {
       System_abort("Error registering button 0 callback function");
    }

    buttonHandle1 = PIN_open(&buttonState1, buttonConfig1);
    if(!buttonHandle1) {
       System_abort("Error initializing button 1 pin\n");
    }

    if (PIN_registerIntCb(buttonHandle1, &buttonFxn) != 0) {
       System_abort("Error registering button 1 callback function");
    }

    BIOS_start();

    return (0);
}
