/* XDCtools files */
#include <main.h>
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
#include "communication.h"


// Sends stop music command wirelessly
void stopMusic() {
    sendString("S");
}

// Sends toggle music command wirelessly
void toggleMusic() {
    sendString("T");
}

// Sends play happy theme command wirelessly
void playHappyTheme() {
    sendString("P1");
}

// Sends play angry theme command wirelessly
void playAngryTheme() {
    sendString("P2");
}

// Sends play warning beep command wirelessly
void playWaningBeep() {
    sendString("P3");
}

// Sends play moving up sting command wirelessly
void playMovingUpSting() {
    sendString("P4");
}

// Sends play shaking sting command wirelessly
void playShakingSting() {
    sendString("P5");
}

// Sends play playing sting command wirelessly
void playPlayingString() {
    sendString("P6");
}

// Sends play pipes effect command wirelessly
void playPipesEffect() {
    sendString("P7");
}

// Sends play game lose sting command wirelessly
void playGameLoseSting() {
    sendString("P8");
}

