/**
 * System Interrupts Generated Driver File
 *
 * @Company:
 *  Microchip Technology Inc.
 *
 * @File Name:
 *  pin_manager.h
 *
 * @Summary:
 *  This is the generated manager file for the MPLAB(c) Code Configurator device.  This manager
 *  configures the pins direction, initial state, analog setting.
 *  The peripheral pin select, PPS, configuration is also handled by this manager.
 *
 * @Description:
 *  This source file provides implementations for MPLAB(c) Code Configurator interrupts.
 *  Generation Information :
 *      Product Revision  :  MPLAB(c) Code Configurator - 4.26
 *      Device            :  PIC24FJ128GB202
 *  The generated drivers are tested against the following:
 *      Compiler          :  XC16 1.31
 *      MPLAB             :  MPLAB X 3.60
 *
 *  Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.
 *
 *  Microchip licenses to you the right to use, modify, copy and distribute
 *  Software only when embedded on a Microchip microcontroller or digital signal
 *  controller that is integrated into your product or third party product
 *  (pursuant to the sublicense terms in the accompanying license agreement).
 *
 *  You should refer to the license agreement accompanying this Software for
 *  additional information regarding your rights and obligations.
 *
 *  SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
 *  EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
 *  MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
 *  IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
 *  CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
 *  OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
 *  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
 *  CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
 *  SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
 *  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *
 */

#ifndef _PIN_MANAGER_H
#define _PIN_MANAGER_H
/**
 *  Section: Includes
 */
#include <xc.h>
/**
 *  Section: Device Pin Macros
 */
/**
 * @Summary
 *  Sets the GPIO pin, RA2, high using LATA2.
 *
 * @Description
 *  Sets the GPIO pin, RA2, high using LATA2.
 *
 * @Preconditions
 *  The RA2 must be set to an output.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Set RA2 high (1)
 *  LEFT_MOTOR_SetHigh();
 *  </code>
 *
 */
#define LEFT_MOTOR_SetHigh()          _LATA2 = 1
/**
 * @Summary
 *  Sets the GPIO pin, RA2, low using LATA2.
 *
 * @Description
 *  Sets the GPIO pin, RA2, low using LATA2.
 *
 * @Preconditions
 *  The RA2 must be set to an output.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Set RA2 low (0)
 *  LEFT_MOTOR_SetLow();
 *  </code>
 *
 */
#define LEFT_MOTOR_SetLow()           _LATA2 = 0
/**
 * @Summary
 *  Toggles the GPIO pin, RA2, using LATA2.
 *
 * @Description
 *  Toggles the GPIO pin, RA2, using LATA2.
 *
 * @Preconditions
 *  The RA2 must be set to an output.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Toggle RA2
 *  LEFT_MOTOR_Toggle();
 *  </code>
 *
 */
#define LEFT_MOTOR_Toggle()           _LATA2 ^= 1
/**
 * @Summary
 *  Reads the value of the GPIO pin, RA2.
 *
 * @Description
 *  Reads the value of the GPIO pin, RA2.
 *
 * @Preconditions
 *  None.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  uint16_t portValue;
 *
 *  // Read RA2
 *  postValue = LEFT_MOTOR_GetValue();
 *  </code>
 *
 */
#define LEFT_MOTOR_GetValue()         _RA2
/**
 * @Summary
 *  Configures the GPIO pin, RA2, as an input.
 *
 * @Description
 *  Configures the GPIO pin, RA2, as an input.
 *
 * @Preconditions
 *  None.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Sets the RA2 as an input
 *  LEFT_MOTOR_SetDigitalInput();
 *  </code>
 *
 */
#define LEFT_MOTOR_SetDigitalInput()  _TRISA2 = 1
/**
 * @Summary
 *  Configures the GPIO pin, RA2, as an output.
 *
 * @Description
 *  Configures the GPIO pin, RA2, as an output.
 *
 * @Preconditions
 *  None.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Sets the RA2 as an output
 *  LEFT_MOTOR_SetDigitalOutput();
 *  </code>
 *
 */
#define LEFT_MOTOR_SetDigitalOutput() _TRISA2 = 0
/**
 * @Summary
 *  Sets the GPIO pin, RA3, high using LATA3.
 *
 * @Description
 *  Sets the GPIO pin, RA3, high using LATA3.
 *
 * @Preconditions
 *  The RA3 must be set to an output.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Set RA3 high (1)
 *  RIGHT_MOTOR_SetHigh();
 *  </code>
 *
 */
#define RIGHT_MOTOR_SetHigh()          _LATA3 = 1
/**
 * @Summary
 *  Sets the GPIO pin, RA3, low using LATA3.
 *
 * @Description
 *  Sets the GPIO pin, RA3, low using LATA3.
 *
 * @Preconditions
 *  The RA3 must be set to an output.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Set RA3 low (0)
 *  RIGHT_MOTOR_SetLow();
 *  </code>
 *
 */
#define RIGHT_MOTOR_SetLow()           _LATA3 = 0
/**
 * @Summary
 *  Toggles the GPIO pin, RA3, using LATA3.
 *
 * @Description
 *  Toggles the GPIO pin, RA3, using LATA3.
 *
 * @Preconditions
 *  The RA3 must be set to an output.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Toggle RA3
 *  RIGHT_MOTOR_Toggle();
 *  </code>
 *
 */
#define RIGHT_MOTOR_Toggle()           _LATA3 ^= 1
/**
 * @Summary
 *  Reads the value of the GPIO pin, RA3.
 *
 * @Description
 *  Reads the value of the GPIO pin, RA3.
 *
 * @Preconditions
 *  None.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  uint16_t portValue;
 *
 *  // Read RA3
 *  postValue = RIGHT_MOTOR_GetValue();
 *  </code>
 *
 */
#define RIGHT_MOTOR_GetValue()         _RA3
/**
 * @Summary
 *  Configures the GPIO pin, RA3, as an input.
 *
 * @Description
 *  Configures the GPIO pin, RA3, as an input.
 *
 * @Preconditions
 *  None.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Sets the RA3 as an input
 *  RIGHT_MOTOR_SetDigitalInput();
 *  </code>
 *
 */
#define RIGHT_MOTOR_SetDigitalInput()  _TRISA3 = 1
/**
 * @Summary
 *  Configures the GPIO pin, RA3, as an output.
 *
 * @Description
 *  Configures the GPIO pin, RA3, as an output.
 *
 * @Preconditions
 *  None.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Sets the RA3 as an output
 *  RIGHT_MOTOR_SetDigitalOutput();
 *  </code>
 *
 */
#define RIGHT_MOTOR_SetDigitalOutput() _TRISA3 = 0
/**
 * @Summary
 *  Sets the GPIO pin, RB11, high using LATB11.
 *
 * @Description
 *  Sets the GPIO pin, RB11, high using LATB11.
 *
 * @Preconditions
 *  The RB11 must be set to an output.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Set RB11 high (1)
 *  RB11_SetHigh();
 *  </code>
 *
 */
#define RB11_SetHigh()          _LATB11 = 1
/**
 * @Summary
 *  Sets the GPIO pin, RB11, low using LATB11.
 *
 * @Description
 *  Sets the GPIO pin, RB11, low using LATB11.
 *
 * @Preconditions
 *  The RB11 must be set to an output.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Set RB11 low (0)
 *  RB11_SetLow();
 *  </code>
 *
 */
#define RB11_SetLow()           _LATB11 = 0
/**
 * @Summary
 *  Toggles the GPIO pin, RB11, using LATB11.
 *
 * @Description
 *  Toggles the GPIO pin, RB11, using LATB11.
 *
 * @Preconditions
 *  The RB11 must be set to an output.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Toggle RB11
 *  RB11_Toggle();
 *  </code>
 *
 */
#define RB11_Toggle()           _LATB11 ^= 1
/**
 * @Summary
 *  Reads the value of the GPIO pin, RB11.
 *
 * @Description
 *  Reads the value of the GPIO pin, RB11.
 *
 * @Preconditions
 *  None.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  uint16_t portValue;
 *
 *  // Read RB11
 *  postValue = RB11_GetValue();
 *  </code>
 *
 */
#define RB11_GetValue()         _RB11
/**
 * @Summary
 *  Configures the GPIO pin, RB11, as an input.
 *
 * @Description
 *  Configures the GPIO pin, RB11, as an input.
 *
 * @Preconditions
 *  None.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Sets the RB11 as an input
 *  RB11_SetDigitalInput();
 *  </code>
 *
 */
#define RB11_SetDigitalInput()  _TRISB11 = 1
/**
 * @Summary
 *  Configures the GPIO pin, RB11, as an output.
 *
 * @Description
 *  Configures the GPIO pin, RB11, as an output.
 *
 * @Preconditions
 *  None.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Sets the RB11 as an output
 *  RB11_SetDigitalOutput();
 *  </code>
 *
 */
#define RB11_SetDigitalOutput() _TRISB11 = 0
/**
 * @Summary
 *  Sets the GPIO pin, RB2, high using LATB2.
 *
 * @Description
 *  Sets the GPIO pin, RB2, high using LATB2.
 *
 * @Preconditions
 *  The RB2 must be set to an output.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Set RB2 high (1)
 *  LEFT_RUDDER_SetHigh();
 *  </code>
 *
 */
#define LEFT_RUDDER_SetHigh()          _LATB2 = 1
/**
 * @Summary
 *  Sets the GPIO pin, RB2, low using LATB2.
 *
 * @Description
 *  Sets the GPIO pin, RB2, low using LATB2.
 *
 * @Preconditions
 *  The RB2 must be set to an output.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Set RB2 low (0)
 *  LEFT_RUDDER_SetLow();
 *  </code>
 *
 */
#define LEFT_RUDDER_SetLow()           _LATB2 = 0
/**
 * @Summary
 *  Toggles the GPIO pin, RB2, using LATB2.
 *
 * @Description
 *  Toggles the GPIO pin, RB2, using LATB2.
 *
 * @Preconditions
 *  The RB2 must be set to an output.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Toggle RB2
 *  LEFT_RUDDER_Toggle();
 *  </code>
 *
 */
#define LEFT_RUDDER_Toggle()           _LATB2 ^= 1
/**
 * @Summary
 *  Reads the value of the GPIO pin, RB2.
 *
 * @Description
 *  Reads the value of the GPIO pin, RB2.
 *
 * @Preconditions
 *  None.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  uint16_t portValue;
 *
 *  // Read RB2
 *  postValue = LEFT_RUDDER_GetValue();
 *  </code>
 *
 */
#define LEFT_RUDDER_GetValue()         _RB2
/**
 * @Summary
 *  Configures the GPIO pin, RB2, as an input.
 *
 * @Description
 *  Configures the GPIO pin, RB2, as an input.
 *
 * @Preconditions
 *  None.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Sets the RB2 as an input
 *  LEFT_RUDDER_SetDigitalInput();
 *  </code>
 *
 */
#define LEFT_RUDDER_SetDigitalInput()  _TRISB2 = 1
/**
 * @Summary
 *  Configures the GPIO pin, RB2, as an output.
 *
 * @Description
 *  Configures the GPIO pin, RB2, as an output.
 *
 * @Preconditions
 *  None.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Sets the RB2 as an output
 *  LEFT_RUDDER_SetDigitalOutput();
 *  </code>
 *
 */
#define LEFT_RUDDER_SetDigitalOutput() _TRISB2 = 0
/**
 * @Summary
 *  Sets the GPIO pin, RB3, high using LATB3.
 *
 * @Description
 *  Sets the GPIO pin, RB3, high using LATB3.
 *
 * @Preconditions
 *  The RB3 must be set to an output.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Set RB3 high (1)
 *  RIGHT_RUDDER_SetHigh();
 *  </code>
 *
 */
#define RIGHT_RUDDER_SetHigh()          _LATB3 = 1
/**
 * @Summary
 *  Sets the GPIO pin, RB3, low using LATB3.
 *
 * @Description
 *  Sets the GPIO pin, RB3, low using LATB3.
 *
 * @Preconditions
 *  The RB3 must be set to an output.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Set RB3 low (0)
 *  RIGHT_RUDDER_SetLow();
 *  </code>
 *
 */
#define RIGHT_RUDDER_SetLow()           _LATB3 = 0
/**
 * @Summary
 *  Toggles the GPIO pin, RB3, using LATB3.
 *
 * @Description
 *  Toggles the GPIO pin, RB3, using LATB3.
 *
 * @Preconditions
 *  The RB3 must be set to an output.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Toggle RB3
 *  RIGHT_RUDDER_Toggle();
 *  </code>
 *
 */
#define RIGHT_RUDDER_Toggle()           _LATB3 ^= 1
/**
 * @Summary
 *  Reads the value of the GPIO pin, RB3.
 *
 * @Description
 *  Reads the value of the GPIO pin, RB3.
 *
 * @Preconditions
 *  None.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  uint16_t portValue;
 *
 *  // Read RB3
 *  postValue = RIGHT_RUDDER_GetValue();
 *  </code>
 *
 */
#define RIGHT_RUDDER_GetValue()         _RB3
/**
 * @Summary
 *  Configures the GPIO pin, RB3, as an input.
 *
 * @Description
 *  Configures the GPIO pin, RB3, as an input.
 *
 * @Preconditions
 *  None.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Sets the RB3 as an input
 *  RIGHT_RUDDER_SetDigitalInput();
 *  </code>
 *
 */
#define RIGHT_RUDDER_SetDigitalInput()  _TRISB3 = 1
/**
 * @Summary
 *  Configures the GPIO pin, RB3, as an output.
 *
 * @Description
 *  Configures the GPIO pin, RB3, as an output.
 *
 * @Preconditions
 *  None.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Sets the RB3 as an output
 *  RIGHT_RUDDER_SetDigitalOutput();
 *  </code>
 *
 */
#define RIGHT_RUDDER_SetDigitalOutput() _TRISB3 = 0
/**
 * @Summary
 *  Sets the GPIO pin, RB5, high using LATB5.
 *
 * @Description
 *  Sets the GPIO pin, RB5, high using LATB5.
 *
 * @Preconditions
 *  The RB5 must be set to an output.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Set RB5 high (1)
 *  STATUS_LED_SetHigh();
 *  </code>
 *
 */
#define STATUS_LED_SetHigh()          _LATB5 = 1
/**
 * @Summary
 *  Sets the GPIO pin, RB5, low using LATB5.
 *
 * @Description
 *  Sets the GPIO pin, RB5, low using LATB5.
 *
 * @Preconditions
 *  The RB5 must be set to an output.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Set RB5 low (0)
 *  STATUS_LED_SetLow();
 *  </code>
 *
 */
#define STATUS_LED_SetLow()           _LATB5 = 0
/**
 * @Summary
 *  Toggles the GPIO pin, RB5, using LATB5.
 *
 * @Description
 *  Toggles the GPIO pin, RB5, using LATB5.
 *
 * @Preconditions
 *  The RB5 must be set to an output.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Toggle RB5
 *  STATUS_LED_Toggle();
 *  </code>
 *
 */
#define STATUS_LED_Toggle()           _LATB5 ^= 1
/**
 * @Summary
 *  Reads the value of the GPIO pin, RB5.
 *
 * @Description
 *  Reads the value of the GPIO pin, RB5.
 *
 * @Preconditions
 *  None.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  uint16_t portValue;
 *
 *  // Read RB5
 *  postValue = STATUS_LED_GetValue();
 *  </code>
 *
 */
#define STATUS_LED_GetValue()         _RB5
/**
 * @Summary
 *  Configures the GPIO pin, RB5, as an input.
 *
 * @Description
 *  Configures the GPIO pin, RB5, as an input.
 *
 * @Preconditions
 *  None.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Sets the RB5 as an input
 *  STATUS_LED_SetDigitalInput();
 *  </code>
 *
 */
#define STATUS_LED_SetDigitalInput()  _TRISB5 = 1
/**
 * @Summary
 *  Configures the GPIO pin, RB5, as an output.
 *
 * @Description
 *  Configures the GPIO pin, RB5, as an output.
 *
 * @Preconditions
 *  None.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Sets the RB5 as an output
 *  STATUS_LED_SetDigitalOutput();
 *  </code>
 *
 */
#define STATUS_LED_SetDigitalOutput() _TRISB5 = 0
/**
 * @Summary
 *  Sets the GPIO pin, RB7, high using LATB7.
 *
 * @Description
 *  Sets the GPIO pin, RB7, high using LATB7.
 *
 * @Preconditions
 *  The RB7 must be set to an output.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Set RB7 high (1)
 *  SPI_CS_SetHigh();
 *  </code>
 *
 */
#define SPI_CS_SetHigh()          _LATB7 = 1
/**
 * @Summary
 *  Sets the GPIO pin, RB7, low using LATB7.
 *
 * @Description
 *  Sets the GPIO pin, RB7, low using LATB7.
 *
 * @Preconditions
 *  The RB7 must be set to an output.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Set RB7 low (0)
 *  SPI_CS_SetLow();
 *  </code>
 *
 */
#define SPI_CS_SetLow()           _LATB7 = 0
/**
 * @Summary
 *  Toggles the GPIO pin, RB7, using LATB7.
 *
 * @Description
 *  Toggles the GPIO pin, RB7, using LATB7.
 *
 * @Preconditions
 *  The RB7 must be set to an output.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Toggle RB7
 *  SPI_CS_Toggle();
 *  </code>
 *
 */
#define SPI_CS_Toggle()           _LATB7 ^= 1
/**
 * @Summary
 *  Reads the value of the GPIO pin, RB7.
 *
 * @Description
 *  Reads the value of the GPIO pin, RB7.
 *
 * @Preconditions
 *  None.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  uint16_t portValue;
 *
 *  // Read RB7
 *  postValue = SPI_CS_GetValue();
 *  </code>
 *
 */
#define SPI_CS_GetValue()         _RB7
/**
 * @Summary
 *  Configures the GPIO pin, RB7, as an input.
 *
 * @Description
 *  Configures the GPIO pin, RB7, as an input.
 *
 * @Preconditions
 *  None.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Sets the RB7 as an input
 *  SPI_CS_SetDigitalInput();
 *  </code>
 *
 */
#define SPI_CS_SetDigitalInput()  _TRISB7 = 1
/**
 * @Summary
 *  Configures the GPIO pin, RB7, as an output.
 *
 * @Description
 *  Configures the GPIO pin, RB7, as an output.
 *
 * @Preconditions
 *  None.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Sets the RB7 as an output
 *  SPI_CS_SetDigitalOutput();
 *  </code>
 *
 */
#define SPI_CS_SetDigitalOutput() _TRISB7 = 0
/**
 * @Summary
 *  Sets the GPIO pin, RB9, high using LATB9.
 *
 * @Description
 *  Sets the GPIO pin, RB9, high using LATB9.
 *
 * @Preconditions
 *  The RB9 must be set to an output.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Set RB9 high (1)
 *  RB9_SetHigh();
 *  </code>
 *
 */
#define RB9_SetHigh()          _LATB9 = 1
/**
 * @Summary
 *  Sets the GPIO pin, RB9, low using LATB9.
 *
 * @Description
 *  Sets the GPIO pin, RB9, low using LATB9.
 *
 * @Preconditions
 *  The RB9 must be set to an output.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Set RB9 low (0)
 *  RB9_SetLow();
 *  </code>
 *
 */
#define RB9_SetLow()           _LATB9 = 0
/**
 * @Summary
 *  Toggles the GPIO pin, RB9, using LATB9.
 *
 * @Description
 *  Toggles the GPIO pin, RB9, using LATB9.
 *
 * @Preconditions
 *  The RB9 must be set to an output.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Toggle RB9
 *  RB9_Toggle();
 *  </code>
 *
 */
#define RB9_Toggle()           _LATB9 ^= 1
/**
 * @Summary
 *  Reads the value of the GPIO pin, RB9.
 *
 * @Description
 *  Reads the value of the GPIO pin, RB9.
 *
 * @Preconditions
 *  None.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  uint16_t portValue;
 *
 *  // Read RB9
 *  postValue = RB9_GetValue();
 *  </code>
 *
 */
#define RB9_GetValue()         _RB9
/**
 * @Summary
 *  Configures the GPIO pin, RB9, as an input.
 *
 * @Description
 *  Configures the GPIO pin, RB9, as an input.
 *
 * @Preconditions
 *  None.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Sets the RB9 as an input
 *  RB9_SetDigitalInput();
 *  </code>
 *
 */
#define RB9_SetDigitalInput()  _TRISB9 = 1
/**
 * @Summary
 *  Configures the GPIO pin, RB9, as an output.
 *
 * @Description
 *  Configures the GPIO pin, RB9, as an output.
 *
 * @Preconditions
 *  None.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  // Sets the RB9 as an output
 *  RB9_SetDigitalOutput();
 *  </code>
 *
 */
#define RB9_SetDigitalOutput() _TRISB9 = 0

/**
 *  Section: Function Prototypes
 */
/**
 * @Summary
 *  Configures the pin settings of the PIC24FJ128GB202
 *  The peripheral pin select, PPS, configuration is also handled by this manager.
 *
 * @Description
 *  This is the generated manager file for the MPLAB(c) Code Configurator device.  This manager
 *  configures the pins direction, initial state, analog setting.
 *  The peripheral pin select, PPS, configuration is also handled by this manager.
 *
 * @Preconditions
 *  None.
 *
 * @Returns
 *  None.
 *
 * @Param
 *  None.
 *
 * @Example
 *  <code>
 *  void SYSTEM_Initialize(void)
 *  {
 *      // Other initializers are called from this function
 *      PIN_MANAGER_Initialize();
 *  }
 *  </code>
 *
 */
void PIN_MANAGER_Initialize(void);

#endif
