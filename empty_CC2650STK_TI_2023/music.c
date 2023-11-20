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
#include "communication.h"
#include "empty.h"


void stopMusic() {
    sendString("S");
}

void toggleMusic() {
    sendString("T");
}

void playHappyTheme() {
    sendString("P1");
}

void playAngryTheme() {
    sendString("P2");
}

void playWaningBeep() {
    sendString("P3");
}

void playMovingUpSting() {
    sendString("P4");
}

void playShakingSting() {
    sendString("P5");
}

void playPlayingString() {
    sendString("P6");
}

void playPipesEffect() {
    sendString("P7");
}

void playGameLoseSting() {
    sendString("P8");
}
