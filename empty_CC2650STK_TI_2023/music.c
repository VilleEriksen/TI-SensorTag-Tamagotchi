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
#include "music.h"

// Task vars

static Clock_Handle musicHandle;
static Clock_Params musicParams;

static PIN_Handle hBuzzer;
static PIN_State sBuzzer;
PIN_Config cBuzzer[] = {
    Board_BUZZER | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    PIN_TERMINATE
};

// Music vars

//static int a[] = {1319, 523, 1319, 523, 1319, 523, 1397, 0, 1175, 392, 1175, 392, 1175, 392, 1319, 0, 1046, 349, 1046, 349, 1175, 392, 1046, 523, 784};

struct song *songStruct;

uint16_t songI;
bool loopMusic;

void musixFxn(UArg arg0) {
    if (songI >= songStruct->length) {
        if (loopMusic) {
            // Return to the starting pointer.
            (songStruct->song) -= songI;
            songI = 0;
        } else {
            buzzerClose();
            Clock_delete(&musicHandle);
            return;
        }
    }

    buzzerSetFrequency(*(songStruct->song));
    (songStruct->song)++;
    songI++;
}

void initMusic() {
    // Buzzer
    hBuzzer = PIN_open(&sBuzzer, cBuzzer);
    if (hBuzzer == NULL) {
        System_abort("Pin open failed!");
    }

    Clock_Params_init(&musicParams);
    musicParams.startFlag = TRUE;
}

void startMusic(struct song *songVar, bool loopMusicVar) {
    songStruct = songVar;
    loopMusic = loopMusicVar;

    buzzerOpen(hBuzzer);

    musicParams.period = songVar->speed / Clock_tickPeriod;
    musicHandle = Clock_create((Clock_FuncPtr)musixFxn, songVar->speed / Clock_tickPeriod, &musicParams, NULL);
    if (musicHandle == NULL) {
        System_abort("Music create failed!");
    }
}
