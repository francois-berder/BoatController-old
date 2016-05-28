/**
  UART1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    uart1.c

  @Summary
    This is the generated driver implementation file for the UART1 driver using MPLAB(c) Code Configurator

  @Description
    This header file provides implementations for driver APIs for UART1.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - v3.00
        Device            :  PIC24FJ128GB202
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC16 1.26
        MPLAB             :  MPLAB X 3.20
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
#include "uart1.h"

/**
  Section: UART1 APIs
*/

void UART1_Initialize(void)
{
    // Set the UART1 module to the options selected in the user interface.

    // STSEL 1; IREN disabled; PDSEL 8N; UARTEN enabled; RTSMD disabled; USIDL disabled; WAKE disabled; ABAUD disabled; LPBACK disabled; BRGH disabled; URXINV disabled; UEN TX_RX; 
    U1MODE = 0x8000;

    // OERR NO_ERROR_cleared; URXISEL RX_ONE_CHAR; UTXBRK COMPLETED; UTXEN disabled; ADDEN disabled; UTXISEL0 TX_ONE_CHAR; UTXINV disabled; 
    U1STA = 0x0000;

    // BaudRate = 38400; Frequency = 16000000 Hz; U1BRG 25; 
    U1BRG = 0x0019;

    // ADMADDR 0; ADMMASK 0; 
    U1ADMD = 0x0000;

    // T0PD 1 ETU; PTRCL T0; TXRPT Retransmits the error byte once; CONV Direct; SCEN disabled; 
    U1SCCON = 0x0000;

    // TXRPTIF disabled; TXRPTIE disabled; WTCIF disabled; WTCIE disabled; PARIE disabled; GTCIF disabled; GTCIE disabled; RXRPTIE disabled; RXRPTIF disabled; 
    U1SCINT = 0x0000;

    // GTC 0; 
    U1GTC = 0x0000;

    // WTCL 0; 
    U1WTCL = 0x0000;

    // WTCH 0; 
    U1WTCH = 0x0000;

    U1STAbits.UTXEN = 1;
}


uint8_t UART1_Read(void)
{
    while(!(U1STAbits.URXDA == 1))
    {
        
    }

    if ((U1STAbits.OERR == 1))
    {
        U1STAbits.OERR = 0;
    }

    

    return U1RXREG;
}

void UART1_Write(uint8_t txData)
{
    while(U1STAbits.UTXBF == 1)
    {
        
    }

    U1TXREG = txData;    // Write the data byte to the USART.
}

UART1_STATUS UART1_StatusGet (void)
{
    return U1STA;
}

int __attribute__((__section__(".libc.write"))) write(int handle, void *buffer, unsigned int len) {
    int i;
    while(U1STAbits.TRMT == 0);  
    for (i = len; i; --i)
    {
        while(U1STAbits.TRMT == 0);
        U1TXREG = *(char*)buffer++;        
    }
    return(len);
}

/**
  End of File
*/
