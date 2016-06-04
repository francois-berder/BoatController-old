/**
  IC1 Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    ic1.h

  @Summary
    This is the generated header file for the IC1 driver using MPLAB(c) Code Configurator

  @Description
    This header file provides APIs for driver for IC1.
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

#ifndef _IC1_H
#define _IC1_H

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
    This function initializes IC instance : 1

  @Description
    This routine initializes the IC driver instance for : 1
    index, making it ready for clients to open and use it.
    This routine must be called before any other IC1 routine is called.
	
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
 
    IC1_Initialize();
    
    IC1_Start();
 
    while(1)
    {
        bufferStatus = IC1_IsCaptureBufferEmpty( void );
        if(!bufferStatus)
        {
            data = IC1_CaptureDataRead();
        }
    }
    IC1_Stop();
 
    </code>

*/

void IC1_Initialize (void);
/**
    void DRV_IC1_Initialize(void)
*/
void DRV_IC1_Initialize(void) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse IC1_Initialize instead. ")));

/**
  @Summary
    Enables the IC module with the corresponding operation mode.

  @Description
    This routine enables the IC module with the corresponding operation mode.

  @Preconditions
    IC1_Initialize function should have been called 

  @Param
    None
 
  @Returns
    None.

  @Example 
    Refer to IC1_Initialize() for an example	
 
*/

void IC1_Start( void );
/**
    void DRV_IC1_Start(void)
*/
void DRV_IC1_Start(void) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse IC1_Start instead. ")));

/**
  @Summary
    Disables the IC module.

  @Description
    This routine disables the IC module.

  @Preconditions
    IC1_Initialize function should have been called 

  @Param
    None
 
  @Returns
    None.

  @Example 
    Refer to IC1_Initialize() for an example	
  	
*/

void IC1_Stop( void );
/**
    void DRV_IC1_Stop(void)
*/
void DRV_IC1_Stop(void) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse IC1_Stop instead. ")));

/**
  @Summary
    Reads the captured data from buffer.

  @Description
    This routine reads the captured data from buffer.

  @Preconditions
    IC1_Initialize function should have been called 

  @Param
    None
 
  @Returns
    Read data from buffer.

  @Example 
    Refer to IC1_Initialize() for an example	
  	
*/

uint16_t IC1_CaptureDataRead( void );
/**
    uint16_t DRV_IC1_CaptureDataRead(void)
*/
uint16_t DRV_IC1_CaptureDataRead(void) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse IC1_CaptureDataRead instead. ")));

/**
  @Summary
    Sets the manual trigger

  @Description
    This routine sets the manual trigger
	
  @Preconditions
    IC1_Initialize function should have been called 

  @Param
    None
 
  @Returns
    None.
	
  @Example 
    Refer to IC1_TriggerStatusGet() for an example	
 
*/
void IC1_ManualTriggerSet( void );

/**
  @Summary
    Gets the status of the timer trigger.

  @Description
    This routine gets the status of the timer trigger source if it has been triggered.
	
  @Preconditions
    IC1_Initialize function should have been called
	
  @Param
    None
 
  @Returns
    Boolean value describing the timer trigger status.
    true  : When the timer source has triggered and is running 
    false : When the timer has not triggered and being held clear 

  @Example 
    <\code>	
    if(IC1_TriggerStatusGet())
    {
        IC1_TriggerStatusClear();
    }
    <\code>	
*/

bool IC1_TriggerStatusGet( void );
/**
    bool DRV_IC1_TriggerStatusGet(void)
*/
bool DRV_IC1_TriggerStatusGet(void) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse IC1_TriggerStatusGet instead. ")));

/**
  @Summary
    Clears the status of the timer trigger.

  @Description
    This routine clears the status of the timer trigger.
	
  @Preconditions
    IC1_Initialize function should have been called 

  @Param
    None
 
  @Returns
    None.
	
  @Example 
    Refer to IC1_TriggerStatusGet() for an example	
 
*/

void IC1_TriggerStatusClear( void );
/**
    void DRV_IC1_TriggerStatusClear(void)
*/
void DRV_IC1_TriggerStatusClear(void) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse IC1_TriggerStatusClear instead. ")));
/**
  @Summary
    Gets the buffer overflow status.

  @Description
    This routine gets the buffer overflow status.
	
  @Preconditions
    IC1_Initialize function should have been called 
	
  @Param
    None
 
  @Returns
    Boolean value describing the buffer overflow status.
    true  : When the capture buffer has overflowed
    false : When the capture buffer has not overflowed

   @Example 
    <\code>	
    bool status;

    status = IC1_HasCaptureBufferOverflowed();
    <\code>	
*/

bool IC1_HasCaptureBufferOverflowed( void );
/**
    bool DRV_IC1_HasCaptureBufferOverflowed(void)
*/
bool DRV_IC1_HasCaptureBufferOverflowed(void) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse IC1_HasCaptureBufferOverflowed instead. ")));

/**
  @Summary
    Gets the buffer empty status.

  @Description
    This routine gets the buffer empty status.
	
  @Preconditions
    IC1_Initialize function should have been called 
	
  @Param
    None

  @Returns
    Boolean value describing the buffer empty status.
    True- If buffer empty
    False-If buffer not empty

   @Example 
    Refer to IC1_Initialize() for an example	
 */

bool IC1_IsCaptureBufferEmpty( void );
/**
    bool DRV_IC1_IsCaptureBufferEmpty(void)
*/
bool DRV_IC1_IsCaptureBufferEmpty(void) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse IC1_IsCaptureBufferEmpty instead. ")));

#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif //_IC1_H
    
/**
 End of File
*/
