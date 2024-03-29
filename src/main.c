/**
 * Generated Main Source File
 *
 * Company:
 *  Microchip Technology Inc.
 *
 * File Name:
 *  main.c
 *
 * Summary:
 *  This is the main file generated using MPLAB(c) Code Configurator
 *
 * Description:
 *  This header file provides implementations for driver APIs for all modules selected in the GUI.
 *  Generation Information :
 *      Product Revision  :  MPLAB(c) Code Configurator - v3.00
 *      Device            :  PIC24FJ128GB202
 *      Driver Version    :  2.00
 *  The generated drivers are tested against the following:
 *      Compiler          :  XC16 1.26
 *      MPLAB             :  MPLAB X 3.20
 */

/*
 * Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.
 *
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital signal
 * controller that is integrated into your product or third party product
 * (pursuant to the sublicense terms in the accompanying license agreement).
 *
 * You should refer to the license agreement accompanying this Software for
 * additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
 * MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
 * CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
 * OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
 * INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
 * SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
 * (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 */

#include "mcc_generated_files/mcc.h"
#include "fat16.h"
#include "hal_sd.h"
#include "log.h"
#include "mbr.h"
#include "sd.h"
#include "status.h"
#include "simple_controller.h"

char log_level = DEBUG;

static void panic(void)
{
    printf("PANIC!\n");
    STATUS_set_mode(STATUS_FAST_BLINK);

    while (1)
        ;
}

static void initialize_sdcard(void)
{
    uint32_t start_sector;

    if (sd_init())
        panic();

    start_sector = mbr_read();
    if (start_sector == 0)
        panic();
    hal_init(start_sector);
    if (fat16_init())
        panic();
}

int main(void)
{
    SYSTEM_Initialize();
    STATUS_set_mode(STATUS_ON);

    initialize_sdcard();

    if (simple_controller_init() < 0)
        panic();

    LOG_INFO("BoatController initialisation finished with success.");
    STATUS_set_mode(STATUS_SLOW_BLINK);

    while (1)
        simple_controller_update();

    return 0;
}
