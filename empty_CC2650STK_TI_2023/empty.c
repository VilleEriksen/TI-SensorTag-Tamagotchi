/*
 * Copyright (c) 2015-2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== empty.c ========
 */
/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>


/* TI-RTOS Header files */
#include <ti/drivers/PIN.h>

/* Board Header files */
#include "Board.h"
#include "menu.h"
#include "display.h"
#include "util/textMenu.h"
#include "empty.h"
#include "gyro.h"
#include "gestureReader.h"

#define BUTTON_0_ID 0
#define BUTTON_1_ID 4

static PIN_Handle buttonHandle0;
static PIN_State buttonState0;

PIN_Config buttonConfig0[] = {
   Board_BUTTON0  | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_NEGEDGE,
   PIN_TERMINATE
};

//global state variables
enum state1 programState = WAITING;

static PIN_Handle buttonHandle1;
static PIN_State buttonState1;

PIN_Config buttonConfig1[] = {
   Board_BUTTON1  | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_NEGEDGE,
   PIN_TERMINATE
};

void buttonFxn(PIN_Handle handle, PIN_Id pinId) {
    switch (pinId) {
        case BUTTON_0_ID:
            display_menuActivate();
            break;
        case BUTTON_1_ID:
            display_menuDown();
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

    initMusic();
    menu_init();

    initDisplay();
    initMPU920();
    initGestureReader();
    //Board_initI2C();

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
