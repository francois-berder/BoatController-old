
/**
  IC2 Generated Driver API Source File

  @Company
    Microchip Technology Inc.

  @File Name
    ic2.c

  @Summary
    This is the generated source file for the IC2 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides APIs for driver for IC2.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - pic24-dspic-pic32mm : v1.35
        Device            :  PIC24FJ128GB202
    The generated drivers are tested against the following:
        Compiler          :  XC16 1.31
        MPLAB 	          :  MPLAB X 3.60
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/
/**
  Section: Included Files
*/
#include <xc.h>
#include "ic2.h"

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
    // ICSIDL disabled; ICM Simple Capture mode: Rising; ICTSEL TMR5; ICI Every; 
    IC2CON1 = 0x0C03;
    // SYNCSEL TMR5; TRIGSTAT disabled; IC32 disabled; ICTRIG Sync; 
    IC2CON2 = 0x000F;
    
    gIC2Mode = IC2CON1bits.ICM;
    
    IFS0bits.IC2IF = false;
    IEC0bits.IC2IE = true;
}


void __attribute__ ( ( interrupt, no_auto_psv ) ) _ISR _IC2Interrupt( void )
{
    if(IFS0bits.IC2IF)
    {
        IFS0bits.IC2IF = 0;
    }
}
void IC2_Start( void )
{
    IC2CON1bits.ICM = gIC2Mode;
}

void IC2_Stop( void )
{
    IC2CON1bits.ICM = 0;
}

uint16_t IC2_CaptureDataRead( void )
{
    return(IC2BUF);
}

void IC2_ManualTriggerSet( void )
{
    IC2CON2bits.TRIGSTAT= true; 
}

bool IC2_TriggerStatusGet( void )
{
    return( IC2CON2bits.TRIGSTAT );
}


void IC2_TriggerStatusClear( void )
{
    /* Clears the trigger status */
    IC2CON2bits.TRIGSTAT = 0;
}
bool IC2_HasCaptureBufferOverflowed( void )
{
    return( IC2CON1bits.ICOV );
}


bool IC2_IsCaptureBufferEmpty( void )
{
    return( ! IC2CON1bits.ICBNE );
}

/**
 End of File
*/
