/**
  IC2 Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    ic2.h

  @Summary
    This is the generated header file for the IC2 driver using MPLAB(c) Code Configurator

  @Description
    This header file provides APIs for driver for IC2.
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

#ifndef _IC2_H
#define _IC2_H

/**
  Section: Included Files
*/

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

/**
  Section: Interface Routines
*/


/**
  @Summary
    This function initializes IC instance : 2

  @Description
    This routine initializes the IC driver instance for : 2
    index, making it ready for clients to open and use it.
    This routine must be called before any other IC2 routine is called.
	
  @Preconditions
    None.

  @Param
    None
 
  @Returns
    None.

  @Example
    <code>
    bool bufferStatus;
    uint16_t data;
 
    IC2_Initialize();
    
    IC2_Start();
 
    while(1)
    {
        bufferStatus = IC2_IsCaptureBufferEmpty( void );
        if(!bufferStatus)
        {
            data = IC2_CaptureDataRead();
        }
    }
    IC2_Stop();
 
    </code>

*/

void IC2_Initialize (void);
/**
    void DRV_IC2_Initialize(void)
*/
void DRV_IC2_Initialize(void) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse IC2_Initialize instead. ")));

/**
  @Summary
    Enables the IC module with the corresponding operation mode.

  @Description
    This routine enables the IC module with the corresponding operation mode.

  @Preconditions
    IC2_Initialize function should have been called 

  @Param
    None
 
  @Returns
    None.

  @Example 
    Refer to IC2_Initialize() for an example	
 
*/

void IC2_Start( void );
/**
    void DRV_IC2_Start(void)
*/
void DRV_IC2_Start(void) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse IC2_Start instead. ")));

/**
  @Summary
    Disables the IC module.

  @Description
    This routine disables the IC module.

  @Preconditions
    IC2_Initialize function should have been called 

  @Param
    None
 
  @Returns
    None.

  @Example 
    Refer to IC2_Initialize() for an example	
  	
*/

void IC2_Stop( void );
/**
    void DRV_IC2_Stop(void)
*/
void DRV_IC2_Stop(void) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse IC2_Stop instead. ")));

/**
  @Summary
    Reads the captured data from buffer.

  @Description
    This routine reads the captured data from buffer.

  @Preconditions
    IC2_Initialize function should have been called 

  @Param
    None
 
  @Returns
    Read data from buffer.

  @Example 
    Refer to IC2_Initialize() for an example	
  	
*/

uint16_t IC2_CaptureDataRead( void );
/**
    uint16_t DRV_IC2_CaptureDataRead(void)
*/
uint16_t DRV_IC2_CaptureDataRead(void) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse IC2_CaptureDataRead instead. ")));

/**
  @Summary
    Sets the manual trigger

  @Description
    This routine sets the manual trigger
	
  @Preconditions
    IC2_Initialize function should have been called 

  @Param
    None
 
  @Returns
    None.
	
  @Example 
    Refer to IC2_TriggerStatusGet() for an example	
 
*/
void IC2_ManualTriggerSet( void );

/**
  @Summary
    Gets the status of the timer trigger.

  @Description
    This routine gets the status of the timer trigger source if it has been triggered.
	
  @Preconditions
    IC2_Initialize function should have been called
	
  @Param
    None
 
  @Returns
    Boolean value describing the timer trigger status.
    true  : When the timer source has triggered and is running 
    false : When the timer has not triggered and being held clear 

  @Example 
    <\code>	
    if(IC2_TriggerStatusGet())
    {
        IC2_TriggerStatusClear();
    }
    <\code>	
*/

bool IC2_TriggerStatusGet( void );
/**
    bool DRV_IC2_TriggerStatusGet(void)
*/
bool DRV_IC2_TriggerStatusGet(void) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse IC2_TriggerStatusGet instead. ")));

/**
  @Summary
    Clears the status of the timer trigger.

  @Description
    This routine clears the status of the timer trigger.
	
  @Preconditions
    IC2_Initialize function should have been called 

  @Param
    None
 
  @Returns
    None.
	
  @Example 
    Refer to IC2_TriggerStatusGet() for an example	
 
*/

void IC2_TriggerStatusClear( void );
/**
    void DRV_IC2_TriggerStatusClear(void)
*/
void DRV_IC2_TriggerStatusClear(void) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse IC2_TriggerStatusClear instead. ")));
/**
  @Summary
    Gets the buffer overflow status.

  @Description
    This routine gets the buffer overflow status.
	
  @Preconditions
    IC2_Initialize function should have been called 
	
  @Param
    None
 
  @Returns
    Boolean value describing the buffer overflow status.
    true  : When the capture buffer has overflowed
    false : When the capture buffer has not overflowed

   @Example 
    <\code>	
    bool status;

    status = IC2_HasCaptureBufferOverflowed();
    <\code>	
*/

bool IC2_HasCaptureBufferOverflowed( void );
/**
    bool DRV_IC2_HasCaptureBufferOverflowed(void)
*/
bool DRV_IC2_HasCaptureBufferOverflowed(void) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse IC2_HasCaptureBufferOverflowed instead. ")));

/**
  @Summary
    Gets the buffer empty status.

  @Description
    This routine gets the buffer empty status.
	
  @Preconditions
    IC2_Initialize function should have been called 
	
  @Param
    None

  @Returns
    Boolean value describing the buffer empty status.
    True- If buffer empty
    False-If buffer not empty

   @Example 
    Refer to IC2_Initialize() for an example	
 */

bool IC2_IsCaptureBufferEmpty( void );
/**
    bool DRV_IC2_IsCaptureBufferEmpty(void)
*/
bool DRV_IC2_IsCaptureBufferEmpty(void) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse IC2_IsCaptureBufferEmpty instead. ")));

#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif //_IC2_H
    
/**
 End of File
*/
