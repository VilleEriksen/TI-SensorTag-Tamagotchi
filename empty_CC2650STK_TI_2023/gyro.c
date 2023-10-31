#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/i2c/I2CCC26XX.h>
#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>

#include <stdio.h>

#include "Board.h"
#include "gyro.h"
#include "util/avgArray.h"
#include "sensors/mpu9250.h"

#define STACKSIZE 2048
Char taskStack[STACKSIZE];

// MPU power pin global variables
static PIN_Handle hMpuPin;
static PIN_State  MpuPinState;

// MPU power pin
static PIN_Config MpuPinConfig[] = {
    Board_MPU_POWER  | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    PIN_TERMINATE
};

// MPU uses its own I2C interface
static const I2CCC26XX_I2CPinCfg i2cMPUCfg = {
    .pinSDA = Board_I2C0_SDA1,
    .pinSCL = Board_I2C0_SCL1
};

struct avgArray* axAvg;
struct avgArray* ayAvg;
struct avgArray* azAvg;
struct avgArray* gxAvg;
struct avgArray* gyAvg;
struct avgArray* gzAvg;

Void sensorFxn(UArg arg0, UArg arg1) {
    float ax, ay, az, gx, gy, gz;
    char printString[50];
    char printString2[50];

    I2C_Handle i2cMPU; // Own i2c-interface for MPU9250 sensor
    I2C_Params i2cMPUParams;

    I2C_Params_init(&i2cMPUParams);
    i2cMPUParams.bitRate = I2C_400kHz;
    // Note the different configuration below
    i2cMPUParams.custom = (uintptr_t)&i2cMPUCfg;

    // MPU power on
    PIN_setOutputValue(hMpuPin, Board_MPU_POWER, Board_MPU_POWER_ON);

    // Wait 100ms for the MPU sensor to power up
    Task_sleep(100000 / Clock_tickPeriod);
    System_printf("MPU9250: Power ON\n");
    System_flush();

    // MPU open i2c
    i2cMPU = I2C_open(Board_I2C, &i2cMPUParams);
    if (i2cMPU == NULL) {
        System_abort("Error Initializing I2CMPU\n");
    }

    // MPU setup and calibration
    System_printf("MPU9250: Setup and calibration...\n");
    System_flush();

    mpu9250_setup(&i2cMPU);

    System_printf("MPU9250: Setup and calibration OK\n");
    System_flush();

    // Loop forever
    while (1) {

        // MPU ask data
        mpu9250_get_data(&i2cMPU, &ax, &ay, &az, &gx, &gy, &gz);

        updateAvgArray(axAvg, ax);
        updateAvgArray(ayAvg, ay);
        updateAvgArray(azAvg, az);
        updateAvgArray(gxAvg, gx);
        updateAvgArray(gyAvg, gy);
        updateAvgArray(gzAvg, gz);

        sprintf(printString, "%.2f, %.2f, %.2f, %.2f, %.2f, %.2f\n\n", axAvg->avg, ayAvg->avg, azAvg->avg, gxAvg->avg, gyAvg->avg, gzAvg->avg);
        System_printf(printString);

        sprintf(printString2, "%.2f, %.2f, %.2f, %.2f, %.2f, %.2f\n\n\n", ax, ay, az, gx, gy, gz);
        System_printf(printString2);
        System_flush();

        // Sleep 100ms
        Task_sleep(100000 / Clock_tickPeriod);
    }

    // Program never gets here..
    // MPU close i2c
    // I2C_close(i2cMPU);
    // MPU power off
    // PIN_setOutputValue(hMpuPin,Board_MPU_POWER, Board_MPU_POWER_OFF);
}

void initMPU920() {
    Task_Handle task;
    Task_Params taskParams;

    // Open MPU power pin
    hMpuPin = PIN_open(&MpuPinState, MpuPinConfig);
    if (hMpuPin == NULL) {
        System_abort("Pin open failed!");
    }

    Task_Params_init(&taskParams);
    taskParams.stackSize = STACKSIZE;
    taskParams.stack = &taskStack;
    task = Task_create((Task_FuncPtr)sensorFxn, &taskParams, NULL);
    if (task == NULL) {
        System_abort("Task create failed!");
    }

    axAvg = createAvgArray(10, 10);
    ayAvg = createAvgArray(10, 10);
    azAvg = createAvgArray(10, 10);
    gxAvg = createAvgArray(10, 10);
    gyAvg = createAvgArray(10, 10);
    gzAvg = createAvgArray(10, 10);

    // Initalize the average arrays.
    /*
    initAvgArray(&axAvg);
    initAvgArray(&ayAvg);
    initAvgArray(&azAvg);
    initAvgArray(&gxAvg);
    initAvgArray(&gyAvg);
    initAvgArray(&gzAvg);
    */
}

/*
void initAvgArray(struct avgArray *avgArrayPar) {
    int i;
    for (i = 0; i < 10; i++) {
        avgArrayPar->arr[i] = i;
    }

    avgArrayPar->avg = 0.0;
    avgArrayPar->i = 0;
}

void updateAvgArray(struct avgArray *avgArrayPar, float val) {
    avgArrayPar->arr[avgArrayPar->i] = val;
    avgArrayPar->i = (avgArrayPar->i + 1) % 10;

    avgArrayPar->avg = calcAvg(avgArrayPar->arr);
}

float calcAvg(float arrPar[]) {
    float avg = 0.0;

    int i;
    for (i = 0; i < 10; i++) {
        avg += arrPar[i];
    }

    avg /= 10.0;

    return avg;
}
*/
