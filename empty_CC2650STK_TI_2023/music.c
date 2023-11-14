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
#include "empty.h"

// Task vars

static Clock_Handle musicHandle;
static Clock_Params musicParams;

static PIN_Handle ledPinHandle;
static PIN_State ledPinState;

static PIN_Handle hBuzzer;
static PIN_State sBuzzer;
PIN_Config cBuzzer[] = {
    Board_BUZZER | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    PIN_TERMINATE
};

PIN_Config ledPinTable[] = {
    Board_LED0 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    PIN_TERMINATE
};

struct song *songStruct;

bool musicEnabled = true;
bool musicPlaying = false;

uint16_t songI;
bool loopMusic;

void musixFxn(UArg arg0) {
    if (songI >= songStruct->length) {
        if (loopMusic) {
            // Return to the starting pointer.
            (songStruct->song) -= songI;
            songI = 0;
        } else {
            PIN_setOutputValue(ledPinHandle, Board_LED0, false);
            buzzerClose();
            Clock_delete(&musicHandle);
            musicPlaying = false;
            return;
        }
    }

    PIN_setOutputValue(ledPinHandle, Board_LED0, !PIN_getOutputValue(Board_LED0));
    buzzerSetFrequency(songStruct->song[songI]);
    songI++;
}

void initMusic() {
    // Buzzer
    hBuzzer = PIN_open(&sBuzzer, cBuzzer);
    if (hBuzzer == NULL) {
        System_abort("Pin open failed!");
    }

    ledPinHandle = PIN_open(&ledPinState, ledPinTable);
    if(!ledPinHandle) {
        System_abort("Error initializing board LED pins\n");
    }

    Clock_Params_init(&musicParams);
    musicParams.startFlag = TRUE;
}

void startMusic(struct song *songVar, bool loopMusicVar) {
    if (musicPlaying || !musicEnabled) return;

    songStruct = songVar;
    loopMusic = loopMusicVar;
    songI = 0;

    buzzerOpen(hBuzzer);

    musicParams.period = songVar->speed / Clock_tickPeriod;
    musicHandle = Clock_create((Clock_FuncPtr)musixFxn, songVar->speed / Clock_tickPeriod, &musicParams, NULL);
    if (musicHandle == NULL) {
        System_abort("Music create failed!");
    }

    musicPlaying = true;
}

void stopMusic() {
    if (musicPlaying) {
        PIN_setOutputValue(ledPinHandle, Board_LED0, false);
        buzzerClose();
        Clock_delete(&musicHandle);
        musicPlaying = false;
    }
}

void toggleMusic() {
    musicEnabled = !musicEnabled;
}

void playHappyTheme() {
    startMusic(&HAPPY_THEME, false);
}

void playAngryTheme() {
    startMusic(&ANGRY_THEME, false);
}

void playWaningBeep() {
    startMusic(&WARNING_BEEP, false);
}

void playMovingUpSting() {
    startMusic(&MOVING_UP_STING, false);
}

void playShakingSting() {
    startMusic(&SHAKING_STING, false);
}

void playPipesEffect() {
    startMusic(&PIPES_EFFECT, false);
}

void playGameLoseSting() {
    startMusic(&GAME_LOSE_STING, false);
}
