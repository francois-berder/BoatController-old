
/**
  IC2 Generated Driver API Source File

  @Company
    Microchip Technology Inc.

  @File Name
    ic2.c

  @Summary
    This is the generated source file for the IC2 driver using MPLAB(c) Code Configurator

  @Description
    This source file provides APIs for driver for IC2.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - v3.00
        Device            :  PIC24FJ128GB202
        Driver Version    :  0,5
    The generated drivers are tested against the following:
        Compiler          :  XC16 1.26
        MPLAB 	          :  MPLAB X 3.20
*/

/*
Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 */
/**
  Section: Included Files
*/
#include <xc.h>
#include "ic2.h"
#include "../radio.h"

/**
  IC Mode.

  @Summary
    Defines the IC Mode.

  @Description
    This data type defines the IC Mode of operation.

*/

static uint16_t         gIC2Mode;

/**
  Section: Driver Interface
*/

void IC2_Initialize (void)
{
    // ICSIDL disabled; ICM Edge Detect Capture; ICTSEL TMR1; ICI Every; 
    IC2CON1 = 0x1001;
    // SYNCSEL None; TRIGSTAT disabled; IC32 disabled; ICTRIG Trigger; 
    IC2CON2 = 0x0080;
    
    gIC2Mode = IC2CON1bits.ICM;
    
    IFS0bits.IC2IF = false;
    IEC0bits.IC2IE = true;
}

/**
    void DRV_IC2_Initialize (void)
*/
void DRV_IC2_Initialize (void)
{
    IC2_Initialize ();
}


void __attribute__ ( ( interrupt, no_auto_psv ) ) _ISR _IC2Interrupt( void )
{
    if(IFS0bits.IC2IF)
    {
        radio_update(POWER_CHANNEL);
        IFS0bits.IC2IF = 0;
    }
}


void IC2_Start( void )
{
    IC2CON1bits.ICM = gIC2Mode;
}

/**
    void DRV_IC2_Start (void)
*/
void DRV_IC2_Start (void)
{
    IC2_Start ();
}

void IC2_Stop( void )
{
    IC2CON1bits.ICM = 0;
}

/**
    void DRV_IC2_Stop (void)
*/
void DRV_IC2_Stop (void)
{
    IC2_Stop ();
}

uint16_t IC2_CaptureDataRead( void )
{
    return(IC2BUF);
}

/**
    uint16_t DRV_IC2_CaptureDataRead (void)
*/
uint16_t DRV_IC2_CaptureDataRead (void)
{
    return(IC2_CaptureDataRead ());
}

void IC2_ManualTriggerSet( void )
{
    IC2CON2bits.TRIGSTAT= true; 
}

bool IC2_TriggerStatusGet( void )
{
    return( IC2CON2bits.TRIGSTAT );
}

/**
    bool DRV_IC2_TriggerStatusGet (void)
*/
bool DRV_IC2_TriggerStatusGet (void)
{
    return(IC2_TriggerStatusGet ());
}

void IC2_TriggerStatusClear( void )
{
    /* Clears the trigger status */
    IC2CON2bits.TRIGSTAT = 0;
}

/**
    void DRV_IC2_TriggerStatusClear (void)
*/
void DRV_IC2_TriggerStatusClear (void)
{
    IC2_TriggerStatusClear ();
}
bool IC2_HasCaptureBufferOverflowed( void )
{
    return( IC2CON1bits.ICOV );
}

/**
    bool DRV_IC2_HasCaptureBufferOverflowed (void)
*/
bool DRV_IC2_HasCaptureBufferOverflowed (void)
{
    return(IC2_HasCaptureBufferOverflowed ());
}

bool IC2_IsCaptureBufferEmpty( void )
{
    return( ! IC2CON1bits.ICBNE );
}

/**
    bool DRV_IC2_IsCaptureBufferEmpty (void)
*/
bool DRV_IC2_IsCaptureBufferEmpty (void)
{
    return(IC2_IsCaptureBufferEmpty ());
}

/**
 End of File
*/
