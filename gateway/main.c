/******************************************************************************
* File:         gateway.c
*
* Author:       Vili Pelttari (University of Oulu) (vilipelttari@gmail.com)
* Created:      18/06/2020
* Description:  General purpose gateway that will forward arbitrary messages
*               from SensorTag 6LoWPAN (max length 116) to UART, and from UART
*               (max length UARTRXSIZE) to 6LoWPAN (max 116).
* Features:
*       - Send UART messages with message delimiter character (EOMCHAR)
*       - 6LoWPAN messages are received in commTask, added to a ring buffer
*         using mutex, and written to UART in uartTask from ring buffer using
*         mutex.
*           * Ring buffer size (RINGBUFSIZE) is 4 kiB, giving space for at
*             minimum 17 messages, 34 max length normal messages, or 1024
*             single-byte messages
*       - UART messages are received using a handler function
*           * Forward messages starting with a SensorTag ID to 6LoWPAN
*           * Request the version of code running on the SensorTag (VERSIONSTR)
*             using the Identity query (confirms the SensorTag after connection)
*           * Request the gateway to respond with a Heartbeat message (HB), to
*             find if it is still running
*
* Version 1.0: Message FIFO queue for temporarily storing messages, Peterson's
*              algorithm for access control to the queue, replies with version
*              string, button press for stopping writing to UART.
* Version 1.1: Sends SensorTag address with version string. uartHandler is made
*              lighter by moving the UART write to a task using a state machine.
* Version 2.0: Reworked UART to use a delimiter when sending to computer. Stops
*              buffer sync errors from happening on the computer because when
*              using a delimiter the buffer will be automatically synced. The
*              heavier commTask could mean dropped messages if they come in too
*              fast. This change also added the RingBuf functionality for
*              storing messages. Relaying from UART to 6LoWPAN was moved to a
*              task for extra stability. Heartbeat messaging implemented to
*              detect crashed gateway.
* Version 2.1: Replace Peterson's algorithm with a Semaphore. The operations
*              with sig_atomic_t are not atomic, so Peterson was not perfect.
*              ARM compiler is also lacking atomic builtin functions. The other
*              end of heartbeat messaging is implemented, to store messages in
*              the buffer if the receiving interface has not sent heartbeats in
*              a while. Even lighter UART handler function.
*****************************************************************************/

#include <inttypes.h>
#include <string.h>
#include <stdio.h>

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>

/* TI-RTOS Header files */
#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/utils/RingBuf.h>

/* Example/Board Header files */
#include "Board.h"
#include "wireless/comm_lib.h"

#include "music.h"

/* Function macros */
#define isInternalMessage(b) (((uint32_t*)b)[0] & 0xffffff) == 0x010000 // b == "\0\0\x01"
#define isAskingForIdentification(b) ((uint32_t*)b)[0] ==  0x6e656449 && ((uint32_t*)b)[1] == 0x79666974 // b == "Identify"
#define isAskingForHeartbeat(b) ((uint16_t*)b)[0] == 0x4248 // b == "HB"

/* Explanation:
 * Due to how the UART delimiter is escaped in message, the maximum length
 * message with SensorTag ID could, for example, be
 *      ESCAPECHAR*2 + ESCAPECHAR*(n-1) + EOMCHAR  -->  ESCAPECHAR*2*(n+1) +
 *      ESCAPECHAR + STANDINCHAR + EOMCHAR,
 * resulting in a message of length 2*(n+1)+3 = 2n+5
 *
 * If more accurate calculation is required, the exact length will be calculated
 * using getEscapedLen. SensorTag ID is already prepended to message, so len + 2
 * is passed.
 */
#define doesItFitInRingBuf(str, len, bufLen) bufLen >= 2*len + 5 || bufLen >= getEscapedLen(str, len + 2)

/* Constant macros */
#define VERSIONSTR "JTKJ SensorTag Server v2.1" // Max length: UART_CUST_TX_MSGLEN - 15
#define COMMSTACKSIZE       3072
#define UARTSTACKSIZE       1024
#define UARTRXLEN           17 // memory for read messages from UART
#define UART_CUST_TX_MSGLEN 50 // memory for writing custom messages to UART
#define RINGBUFSIZE         4096 // ring buffer memory for message buffering
#define TASKSLEEP           5000 // sleep between each run of the uartFxn
#define HEARTBEATINTERVAL   15000 // interval between received heartbeat messages (should match Interface!)

// These should be in sequence
#define ESCAPECHAR  '\xf0' // Escape character
#define STANDINCHAR '\xf1' // Character that replaces the EOMCHAR within message, with the ESCAPECHAR
#define EOMCHAR     '\xf2' // Message end character

/* Functions */
static void uartHandler(UART_Handle handle, void *rxBuf, size_t size);
void addToRingBuff(uint8_t *str, int8_t len);
int16_t getEscapedLen(uint8_t *buf, int16_t bufLen);

/* Variables */
Char uartTaskStack[UARTSTACKSIZE];
Char commTaskStack[COMMSTACKSIZE];
unsigned char dataBuf[RINGBUFSIZE];
RingBuf_Object msgBuf;
RingBuf_Object *msgBufP;
Semaphore_Struct semStruct;
Semaphore_Handle semHandle;

bool connected = false;

/* Pin Button0 configured as input */
static PIN_Handle buttonHandle;
static PIN_State buttonState;
PIN_Config button[] = { // Activate on both KEY_DOWN and KEY_UP events
    Board_BUTTON0  | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_BOTHEDGES,
    PIN_TERMINATE
};

/* Global memory storage for a PIN_Config table */
static PIN_Handle ledPinHandle;
static PIN_State ledPinState;

/*
 * Application LED pin configuration table:
 *   - All board LEDs are off.
 */
PIN_Config ledPinTable[] = {
    Board_LED1 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    Board_LED2 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    PIN_TERMINATE
};

int arg = 0;
uint8_t txBuf[UART_CUST_TX_MSGLEN];
uint16_t txAddr;
bool uartDisabled = false;
uint8_t rxBuf[UARTRXLEN];

enum uartState {WAIT=1, SENDMSG, SENDID, SENDHEARTBEAT} state = WAIT;

static void uartHandler(UART_Handle handle, void *rxBuf, size_t size) {
    // Flicker led
    uint8_t tempbuf[UARTRXLEN];
    PIN_setOutputValue(ledPinHandle, Board_LED1, 1);
    memcpy(tempbuf,rxBuf,UARTRXLEN);
    // Parse message
    if (isInternalMessage(rxBuf)) { // Message starts with "\0\0\x01"
        if (isAskingForHeartbeat(&((char *)rxBuf)[3])) { // Message continues with "HB"
            // Reply to Heartbeat
            state = SENDHEARTBEAT;
        } else if (isAskingForIdentification(&((char *)rxBuf)[3])) { // Message continues with "Identify"
            // Reply to challenge-response
            state = SENDID;
        }
    } else if (size > 2) { // Don't allow empty messages. Fixes a crash with memcpy on startup
        // Send message over 6LoWPAN
        state = SENDMSG; // Actual send is done in uartFxn
    }
    // Clear UART RX buffer
    UART_control(handle, UART_CMD_RESERVED + 2, &arg);
    // Flicker led  //}, 3000);
    PIN_setOutputValue(ledPinHandle, Board_LED1, 0);
    // Start a new read
    UART_read(handle, rxBuf, UARTRXLEN);
}

/* UART Task */
Void uartFxn(UArg arg0, UArg arg1) {

    //uint8_t rxBuf[UARTRXLEN];
    UART_Handle uart;
    UART_Params uartParams;
    unsigned char c;
    uint32_t iterc = 0; // iteration count (HB)
    uint32_t maxiterc = HEARTBEATINTERVAL / TASKSLEEP * 1500; // HB/ts*1.5*1000 => expected maximum iteration count

    /* Create a UART with data processing off. */
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readMode = UART_MODE_CALLBACK;
    uartParams.readCallback = uartHandler;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 57600;
    uartParams.dataLength = UART_LEN_8; // 8
    uartParams.parityType = UART_PAR_NONE; // n
    uartParams.stopBits = UART_STOP_ONE; // 1
 
    uart = UART_open(Board_UART0, &uartParams);
    if (uart == NULL) {
        System_abort("Error opening the UART connection");
    }
    memset(rxBuf, 0, UARTRXLEN);
    UART_read(uart, rxBuf, UARTRXLEN); // set read handler
    
    msgBufP = &msgBuf;
    RingBuf_construct(msgBufP, dataBuf, RINGBUFSIZE);

    while (1) {

        iterc++;
        Task_sleep(TASKSLEEP / Clock_tickPeriod);

        if (state != WAIT) {
            switch (state) {
            case SENDID:
                connected = true;
                iterc = 0;
                sprintf((char *)txBuf, "\xfe\xfe\x01%s, addr 0x%04x\xf2", VERSIONSTR, IEEE80154_MY_ADDR);
                UART_write(uart, txBuf, strlen((const char*)txBuf));
                state = WAIT;
                break;
            case SENDHEARTBEAT:
                connected = true;
                iterc = 0;
                sprintf((char *)txBuf, "\xfe\xfe\x01HB\xf2");
                UART_write(uart, txBuf, strlen((const char*)txBuf));
                state = WAIT;
                break;
            case SENDMSG:
                // SensorTag ID is two bytes in beginning, in little-endian
                Send6LoWPAN(((uint16_t*)rxBuf)[0], &rxBuf[2], strlen((const char*)&rxBuf[2]));
                StartReceive6LoWPAN();
                state = WAIT;
                break;
            }
        }

        // If Heartbeat has not been heard for a while, set connected to false
        if (iterc > maxiterc) connected = false;

        // Is there anything to send, is the gateway connected, or is the uart disabled
        if (RingBuf_getCount(msgBufP) == 0 || !connected || uartDisabled) continue;

        // sending message over UART. Set the red led on
        PIN_setOutputValue(ledPinHandle, Board_LED1, 1);

        // Set mutex
        Semaphore_pend(semHandle, BIOS_WAIT_FOREVER);

        // Critical section
        // Send a character at a time
        do {
            RingBuf_get(msgBufP, &c);
            UART_write(uart, &c, 1); // write to UART
        } while (c != EOMCHAR);

        // Release mutex
        Semaphore_post(semHandle);

        PIN_setOutputValue(ledPinHandle, Board_LED1, 0); // Sent. Set red led off
    }
}

/* Communication Task */
Void commTaskFxn(UArg arg0, UArg arg1) {

    // Radio to receive mode
	int32_t result = StartReceive6LoWPAN();
	if(result != true) {
		System_abort("Wireless receive mode failed");
	}

    // comm_lib has the length as int8_t so the biggest message we could handle has 127 characters
	uint8_t recBuf[130]; // 130 to add space for senderAddr (two bytes)
	int8_t len;

	System_printf("Receiving messages.\n");
	System_flush();

    while (1) {

        // If true, we have a message
    	if (GetRXFlag()) {

    	    // Set mutex
    	    Semaphore_pend(semHandle, BIOS_WAIT_FOREVER);

            // Critical section
            // Handle the received message..
    	    len = Receive6LoWPAN((uint16_t *)recBuf, (char *)&recBuf[2], 128);
    	    if (len >= 0) {
    	        if (recBuf[2] == 'S') stopMusic();
    	        else if (recBuf[2] == 'T') toggleMusic();
    	        else if (recBuf[2] == 'P' && recBuf[3] == '1') playHappyTheme();
                else if (recBuf[2] == 'P' && recBuf[3] == '2') playAngryTheme();
                else if (recBuf[2] == 'P' && recBuf[3] == '3') playWaningBeep();
                else if (recBuf[2] == 'P' && recBuf[3] == '4') playMovingUpSting();
                else if (recBuf[2] == 'P' && recBuf[3] == '5') playShakingSting();
                else if (recBuf[2] == 'P' && recBuf[3] == '6') playPlayingString();
                else if (recBuf[2] == 'P' && recBuf[3] == '7') playPipesEffect();
                else if (recBuf[2] == 'P' && recBuf[3] == '8') playGameLoseSting();
    	        else {
    	            addToRingBuff(recBuf, len); // len can be -1 to stop overflow
    	        }
    	    }

    	    // Release mutex
    	    Semaphore_post(semHandle);
        }

    	// Absolutely NO Task_sleep in this task!!
    }
}

// Add the string str of length 16 bits of address + len in bytes to the message
// buffer. This will ensure the UART message delimiter will only occur in the
// end of the message
/* Description of operation:
 * Choose a byte
 *
 * Not EOMCHAR:
 *      1. Add the byte to RingBuf
 *      2. If STANDINCHAR (the one that will replace EOMCHAR in message if
 *         encountered)
 *          true:  Write extra ESCAPECHARs equal to the number (escapeLen) that
 *                 came before it (total 2*escapeLen)
 *          false: If ESCAPECHAR (used for differentiating between a real
 *                 STANDINCHAR and EOM)
 *              true:  increment escapeLen (next char might need extra escaping)
 *              false: escapeLen = 0 (next char doesn't need extra escaping)
 * Is EOMCHAR:
 *      1. Write extra ESCAPECHARs equal to the number (escapeLen) that came
 *         before it
 *      2. Write an ESCAPECHAR (makes the total 2*escapeLen + 1)
 *      3. Write STANDINCHAR
 *
 * EOMCHAR and STANDINCHAR are differentiated by the number of ESCAPECHARs before the STANDINCHAR (2k+1 vs. 2k, k∈ℕ)
 */
void addToRingBuff(uint8_t *str, int8_t len) {
    uint8_t c, escapeLen = 0;
    uint16_t bufLen, k;
    // Check if str will fit into the ringbuffer:
    bufLen = RINGBUFSIZE - RingBuf_getCount(msgBufP);
    // Heuristic using 2*(len + senderAddr)+1 before calculating the actual length in buffer
    if (doesItFitInRingBuf(str, len, bufLen)) {
        // Encode message into the RingBuffer
        for (k = 0; k < len + 2; k++) { // Do for every byte in senderAddr + msg
            c = str[k];
            if (c == EOMCHAR) { // If EOMCHAR is in message, escape it with ESC+STANDIN
                while (escapeLen) { // Write extra an k ESCAPECHARs that already came before it
                    RingBuf_put(msgBufP, ESCAPECHAR);
                    escapeLen--;
                }
                RingBuf_put(msgBufP, ESCAPECHAR);
                // The difference between EOM and STANDIN is that EOM has 2k+1 ESCAPECHARs before the STANDIN
                RingBuf_put(msgBufP, STANDINCHAR);
            } else { // if not EOMCHAR
                if (c == STANDINCHAR) { // if STANDINCHAR, write an extra k ESCAPECHARs before writing it to the buffer
                    while (escapeLen) { // Write ESCAPECHARs that came before it
                        RingBuf_put(msgBufP, ESCAPECHAR);
                        escapeLen--;
                    } // STANDINCHAR has 2k ESCAPECHARS before it
                } else { // Standard case
                    // Not a special character -> escapeLen=0; Is a special character -> add 1 to number of ESCs
                    if (c == ESCAPECHAR) escapeLen++;
                    else escapeLen = 0;
                }
                RingBuf_put(msgBufP, c);  // if not EOMCHAR, always put the character in buffer
            }
        }
        RingBuf_put(msgBufP, EOMCHAR); // End every message with EOMCHAR
    }
}

// Calculate the length of the string buf of length bufLen if it were put in the RingBuffer, including EOMCHAR
int16_t getEscapedLen(uint8_t *buf, int16_t bufLen) {
    uint8_t c;
    int16_t escapeLen = 0, len = 1, k; // 1 for EOMCHAR
    for (k = 0; k < bufLen; k++) {
        c = buf[k];
        // If character is special
        if (c >= STANDINCHAR && c <= EOMCHAR) { // get rid of most characters with the first comparison
            len += escapeLen; // escapeLen ESCAPECHARs is written to make 2*escapeLen
            if (c == EOMCHAR) len++; // an extra ESCAPECHAR to make 2*escapeLen + 1
        }
        if (c == ESCAPECHAR) escapeLen++;
        else escapeLen = 0;
        len++;
    }
    return len;
}

void buttonFxn(PIN_Handle handle, PIN_Id pinId) {
    // Pause UART sending while button is pressed down
    uartDisabled = !PIN_getInputValue(Board_BUTTON0);
    PIN_setOutputValue(ledPinHandle, Board_LED2, !uartDisabled);
}

/*
 *  ======== main ========
 */
int main(void) {

    // Task variables
	Task_Handle commTask;
	Task_Params commTaskParams;
	Task_Handle uartTask;
    Task_Params uartTaskParams;
    Semaphore_Params semParams;

    /* Call board init functions */
    Board_initGeneral();
    Board_initUART();

    initMusic();

    /* Construct BIOS objects */
    Init6LoWPAN();

    System_printf("Start\n");
    System_flush();

    /* Semaphore */
    Semaphore_Params_init(&semParams);
    Semaphore_construct(&semStruct, 1, &semParams);
    semHandle = Semaphore_handle(&semStruct);

    /* Communication Task */
    Task_Params_init(&commTaskParams);
    commTaskParams.stackSize = COMMSTACKSIZE;
    commTaskParams.stack = &commTaskStack;
    commTaskParams.priority = 1;

    commTask = Task_create(commTaskFxn, &commTaskParams, NULL);
    if (commTask == NULL) {
    	System_abort("Task create failed!");
    }

    /* UART Task */
    Task_Params_init(&uartTaskParams);
    uartTaskParams.stackSize = UARTSTACKSIZE;
    uartTaskParams.stack = &uartTaskStack;
    uartTaskParams.priority = 2;

    uartTask = Task_create(uartFxn, &uartTaskParams, NULL);
    if (uartTask == NULL) {
    	System_abort("UART task create failed!");
    }

    /* Button pins */
    buttonHandle = PIN_open(&buttonState, button);
    if(!buttonHandle) {
        System_abort("Error initializing button up pins\n");
    }
    /* Setup callback for button pins */
    if (PIN_registerIntCb(buttonHandle, &buttonFxn) != 0) {
        System_abort("Error registering button release callback function");
    }

    /* Open LED pins */
    ledPinHandle = PIN_open(&ledPinState, ledPinTable);
    if(!ledPinHandle) {
        System_abort("Error initializing board LED pins\n");
    }

    PIN_setOutputValue(ledPinHandle, Board_LED2, 1);

    /* Start BIOS */
    BIOS_start();

    return (0);
}
