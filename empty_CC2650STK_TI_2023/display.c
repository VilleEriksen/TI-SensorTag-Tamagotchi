/*
 * display.c
 *
 *  Created on: 31 Oct 2023
 *      Author: Ville
 */

#include <ti/mw/display/Display.h>

Display_Handle displayHandle;

void initDisplay() {
    Display_Params params;
    Display_Params_init(&params);
    params.lineClearMode = DISPLAY_CLEAR_BOTH;

    Display_Handle displayHandle = Display_open(Display_Type_LCD, &params);

    Display_print0(displayHandle, 0, 0, "Testing1");
    Display_print0(displayHandle, 1, 0, "Testing2");
}

