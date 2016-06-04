
/**
  TMR1 Generated Driver API Source File 

  @Company
    Microchip Technology Inc.

  @File Name
    tmr1.c

  @Summary
    This is the generated source file for the TMR1 driver using MPLAB(c) Code Configurator

  @Description
    This source file provides APIs for driver for TMR1. 
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
#include "tmr1.h"

/**
  Section: Data Type Definitions
*/

/** TMR Driver Hardware Instance Object

  @Summary
    Defines the object required for the maintainence of the hardware instance.

  @Description
    This defines the object required for the maintainence of the hardware
    instance. This object exists once per hardware instance of the peripheral.

  Remarks:
    None.
*/

typedef struct _TMR_OBJ_STRUCT
{
    /* Timer Elapsed */
    bool                                                    timerElapsed;
    /*Software Counter value*/
    uint8_t                                                 count;

} TMR_OBJ;

static TMR_OBJ tmr1_obj;

/**
  Section: Driver Interface
*/


void TMR1_Initialize (void)
{
    //TMR1 0; 
    TMR1 = 0x0000;
    //Period = 0,0327675 s; Frequency = 16000000 Hz; PR1 65535; 
    PR1 = 0xFFFF;
    //TCKPS 1:8; TON enabled; TSIDL disabled; TCS FOSC/2; TECS SOSC; TSYNC disabled; TGATE disabled; 
    T1CON = 0x8010;

    
	
    tmr1_obj.timerElapsed = false;

}

/**
    void DRV_TMR1_Initialize (void)
*/
void DRV_TMR1_Initialize (void)
{
    TMR1_Initialize ();
}



void TMR1_Tasks_16BitOperation( void )
{
    /* Check if the Timer Interrupt/Status is set */
    if(IFS0bits.T1IF)
    {
        tmr1_obj.count++;
        tmr1_obj.timerElapsed = true;
        IFS0bits.T1IF = false;
    }
}

/**
    void DRV_TMR1_Tasks_16BitOperation (void)
*/
void DRV_TMR1_Tasks_16BitOperation (void)
{
    TMR1_Tasks_16BitOperation ();
}

void TMR1_Period16BitSet( uint16_t value )
{
    /* Update the counter values */
    PR1 = value;
    /* Reset the status information */
    tmr1_obj.timerElapsed = false;
}

/**
    void DRV_TMR1_Period16BitSet (uint16_t value)
*/
void DRV_TMR1_Period16BitSet (uint16_t value)
{
    TMR1_Period16BitSet (value);
}

uint16_t TMR1_Period16BitGet( void )
{
    return( PR1 );
}

/**
    uint16_t DRV_TMR1_Period16BitGet (void)
*/
uint16_t DRV_TMR1_Period16BitGet (void)
{
    return(TMR1_Period16BitGet ());
}

void TMR1_Counter16BitSet ( uint16_t value )
{
    /* Update the counter values */
    TMR1 = value;
    /* Reset the status information */
    tmr1_obj.timerElapsed = false;
}

/**
    void DRV_TMR1_Counter16BitSet (uint16_t value)
*/
void DRV_TMR1_Counter16BitSet (uint16_t value)
{
    TMR1_Counter16BitSet (value);
}

uint16_t TMR1_Counter16BitGet( void )
{
    return( TMR1 );
}

/**
    uint16_t DRV_TMR1_Counter16BitGet (void)
*/
uint16_t DRV_TMR1_Counter16BitGet (void)
{
    return(TMR1_Counter16BitGet ());
}



void TMR1_Start( void )
{
    /* Reset the status information */
    tmr1_obj.timerElapsed = false;


    /* Start the Timer */
    T1CONbits.TON = 1;
}

/**
    void DRV_TMR1_Start (void)
*/
void DRV_TMR1_Start (void)
{
    TMR1_Start ();
}

void TMR1_Stop( void )
{
    /* Stop the Timer */
    T1CONbits.TON = false;

}

/**
    void DRV_TMR1_Stop (void)
*/
void DRV_TMR1_Stop (void)
{
    TMR1_Stop ();
}

bool TMR1_GetElapsedThenClear(void)
{
    bool status;
    
    status = tmr1_obj.timerElapsed;

    if(status == true)
    {
        tmr1_obj.timerElapsed = false;
    }
    return status;
}

/**
    bool DRV_TMR1_GetElapsedThenClear (void)
*/
bool DRV_TMR1_GetElapsedThenClear (void)
{
    return(TMR1_GetElapsedThenClear ());
}

int TMR1_SoftwareCounterGet(void)
{
    return tmr1_obj.count;
}

/**
    int DRV_TMR1_SoftwareCounterGet (void)
*/
int DRV_TMR1_SoftwareCounterGet (void)
{
    return(TMR1_SoftwareCounterGet ());
}

void TMR1_SoftwareCounterClear(void)
{
    tmr1_obj.count = 0; 
}

/**
    void DRV_TMR1_SoftwareCounterClear (void)
*/
void DRV_TMR1_SoftwareCounterClear (void)
{
    TMR1_SoftwareCounterClear ();
}

/**
 End of File
*/
