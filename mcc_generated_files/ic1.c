
/**
  IC1 Generated Driver API Source File

  @Company
    Microchip Technology Inc.

  @File Name
    ic1.c

  @Summary
    This is the generated source file for the IC1 driver using MPLAB(c) Code Configurator

  @Description
    This source file provides APIs for driver for IC1.
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
#include "ic1.h"
#include "../radio.h"

/**
  IC Mode.

  @Summary
    Defines the IC Mode.

  @Description
    This data type defines the IC Mode of operation.

*/

static uint16_t         gIC1Mode;

/**
  Section: Driver Interface
*/

void IC1_Initialize (void)
{
    // ICSIDL disabled; ICM Edge Detect Capture; ICTSEL FOSC/2; ICI Every; 
    IC1CON1 = 0x1C01;
    // SYNCSEL None; TRIGSTAT disabled; IC32 disabled; ICTRIG Sync; 
    IC1CON2 = 0x0000;
    
    gIC1Mode = IC1CON1bits.ICM;
    
    IFS0bits.IC1IF = false;
    IEC0bits.IC1IE = true;
}

/**
    void DRV_IC1_Initialize (void)
*/
void DRV_IC1_Initialize (void)
{
    IC1_Initialize ();
}


void __attribute__ ( ( interrupt, no_auto_psv ) ) _ISR _IC1Interrupt( void )
{
    if(IFS0bits.IC1IF)
    {
        radio_update(DIRECTION_CHANNEL);
        IFS0bits.IC1IF = 0;
    }
}


void IC1_Start( void )
{
    IC1CON1bits.ICM = gIC1Mode;
}

/**
    void DRV_IC1_Start (void)
*/
void DRV_IC1_Start (void)
{
    IC1_Start ();
}

void IC1_Stop( void )
{
    IC1CON1bits.ICM = 0;
}

/**
    void DRV_IC1_Stop (void)
*/
void DRV_IC1_Stop (void)
{
    IC1_Stop ();
}

uint16_t IC1_CaptureDataRead( void )
{
    return(IC1BUF);
}

/**
    uint16_t DRV_IC1_CaptureDataRead (void)
*/
uint16_t DRV_IC1_CaptureDataRead (void)
{
    return(IC1_CaptureDataRead ());
}

void IC1_ManualTriggerSet( void )
{
    IC1CON2bits.TRIGSTAT= true; 
}

bool IC1_TriggerStatusGet( void )
{
    return( IC1CON2bits.TRIGSTAT );
}

/**
    bool DRV_IC1_TriggerStatusGet (void)
*/
bool DRV_IC1_TriggerStatusGet (void)
{
    return(IC1_TriggerStatusGet ());
}

void IC1_TriggerStatusClear( void )
{
    /* Clears the trigger status */
    IC1CON2bits.TRIGSTAT = 0;
}

/**
    void DRV_IC1_TriggerStatusClear (void)
*/
void DRV_IC1_TriggerStatusClear (void)
{
    IC1_TriggerStatusClear ();
}
bool IC1_HasCaptureBufferOverflowed( void )
{
    return( IC1CON1bits.ICOV );
}

/**
    bool DRV_IC1_HasCaptureBufferOverflowed (void)
*/
bool DRV_IC1_HasCaptureBufferOverflowed (void)
{
    return(IC1_HasCaptureBufferOverflowed ());
}

bool IC1_IsCaptureBufferEmpty( void )
{
    return( ! IC1CON1bits.ICBNE );
}

/**
    bool DRV_IC1_IsCaptureBufferEmpty (void)
*/
bool DRV_IC1_IsCaptureBufferEmpty (void)
{
    return(IC1_IsCaptureBufferEmpty ());
}

/**
 End of File
*/
