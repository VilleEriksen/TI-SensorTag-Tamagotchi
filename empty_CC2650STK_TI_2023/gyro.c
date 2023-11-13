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
#include "empty.h"


#define STACKSIZE 1536
Char taskStack[STACKSIZE];

extern enum state1 programState;

// MPU power pin global variables
static PIN_Handle hMpuPin;
static PIN_State  MpuPinState;

int gyroUpdateSpeed;

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

float ax, ay, az, gx, gy, gz;

double time;

Void sensorFxn(UArg arg0, UArg arg1) {
    //char printString[55];
    //char printString2[55];

    //float shake = 0;

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
        programState = GYRO_READ;

        time = (double)Clock_getTicks() / 100000.0;

        // MPU ask data
        mpu9250_get_data(&i2cMPU, &ax, &ay, &az, &gx, &gy, &gz);

        updateAvgArray(axAvg, ax);
        updateAvgArray(ayAvg, ay);
        updateAvgArray(azAvg, az);
        updateAvgArray(gxAvg, gx);
        updateAvgArray(gyAvg, gy);
        updateAvgArray(gzAvg, gz);

        // sprintf(printString, "%.5f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f\n", time, axAvg->avg, ayAvg->avg, azAvg->avg, gxAvg->avg, gyAvg->avg, gzAvg->avg);
        // System_printf(printString);

        // sprintf(printString2, "%.5f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f\n\n", time, ax, ay, az, gx, gy, gz);
        // System_printf(printString2);
        // System_flush();
        programState = GYRO_DATA_READY;
        // Sleep 100ms
        Task_sleep(gyroUpdateSpeed / Clock_tickPeriod);
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

    gyroUpdateSpeed = 100000;

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

    axAvg = createAvgArray(5, 0);
    ayAvg = createAvgArray(5, 0);
    azAvg = createAvgArray(5, 0);
    gxAvg = createAvgArray(5, 0);
    gyAvg = createAvgArray(5, 0);
    gzAvg = createAvgArray(5, 0);
}
