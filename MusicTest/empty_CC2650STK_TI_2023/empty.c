/* XDCtools files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>

#include "Board.h"
#include "music.h"

Int main(void) {
    // Initialize board
    Board_initGeneral();

    /* Sanity check */
    System_printf("Beeps!\n");
    System_flush();

    initMusic();
    startMusic();

    /* Start BIOS */
    BIOS_start();

    return (0);
}
