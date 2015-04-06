/**
  SPI1 Generated Driver API Header File

  Company:
    Microchip Technology Inc.

  File Name:
    spi1.h

  @Summary
    This is the generated header file for the SPI1 driver using MPLAB® Code Configurator

  @Description
    This header file provides APIs for driver for SPI1.
    Generation Information :
        Product Revision  :  MPLAB® Code Configurator - v2.10.2
        Device            :  PIC24FJ128GC006
        Driver Version    :  0.5
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.24
        MPLAB 	          :  MPLAB X 2.26
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

#ifndef _SPI1_H
#define _SPI1_H

/**
 Section: Included Files
*/

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

/**
 Section: Data Type Definitions
*/
        
/**
  SPI1_DUMMY_DATA 

  @Summary
    Dummy data to be sent. 

  @Description
    Dummy data to be sent, when no input buffer is specified in the buffer APIs.
 */
#define SPI1_DUMMY_DATA 0x0
        
/**
Deprecated Data Type
*/
#define DRV_SPI1_DUMMY_DATA 0x0
		
/**
  SPI1_FIFO_FILL_LIMIT

  @Summary
    FIFO fill limit for data transmission. 

  @Description
    The amount of data to be filled in the FIFO during transmission. The maximum limit allowed is 8.
 */
#define SPI1_FIFO_FILL_LIMIT 0x8

//Check to make sure that the FIFO limit does not exceed the maximum allowed limit of 8
#if (SPI1_FIFO_FILL_LIMIT > 8)

    #define SPI1_FIFO_FILL_LIMIT 8

#endif
	
/**
Deprecated FIFO FILL LIMIT
*/
#define DRV_SPI1_FIFO_FILL_LIMIT 0x8	

#if (DRV_SPI1_FIFO_FILL_LIMIT > 8)

    #define DRV_SPI1_FIFO_FILL_LIMIT 8

#endif

/**
  SPI1 Status Enumeration

  @Summary
    Defines the status enumeration for SPI1.

  @Description
    This defines the status enumeration for SPI1.
 */
typedef enum {
    SPI1_SHIFT_REGISTER_EMPTY  = 1 << 7,
    SPI1_RECEIVE_OVERFLOW = 1 << 6,
    SPI1_RECEIVE_FIFO_EMPTY = 1 < 5,
    SPI1_TRANSMIT_BUFFER_FULL = 1 << 1,
    SPI1_RECEIVE_BUFFER_FULL = 1 << 0
}SPI1_STATUS;

/**
Deprecated Status Enumeration
*/
typedef enum {
    DRV_SPI1_SHIFT_REGISTER_EMPTY  = 1 << 7,
    DRV_SPI1_RECEIVE_OVERFLOW = 1 << 6,
    DRV_SPI1_RECEIVE_FIFO_EMPTY = 1 < 5,
    DRV_SPI1_TRANSMIT_BUFFER_FULL = 1 << 1,
    DRV_SPI1_RECEIVE_BUFFER_FULL = 1 << 0
}DRV_SPI1_STATUS;

/**
  SPI1 Mode Enumeration

  @Summary
    Defines the mode of operation for SPI1.

  @Description
    This defines the mode of operation for SPI1.
 */
typedef enum {
    SPI1_DRIVER_TRANSFER_MODE_8BIT   = 0,
    SPI1_DRIVER_TRANSFER_MODE_16BIT  = 1,
    SPI1_DRIVER_TRANSFER_MODE_32BIT  = 2,
} SPI1_TRANSFER_MODE;

/**
 Section: Interface Routines
*/

/**
  @Summary
    Initializes the SPI instance : 1

  @Description
    This routine initializes the spi1 driver instance for : 1
    index, making it ready for clients to open and use it.

    This routine must be called before any other SPI1 routine is called.
    This routine should only be called once during system initialization.
 
  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t   myWriteBuffer[MY_BUFFER_SIZE];
    uint16_t   myReadBuffer[MY_BUFFER_SIZE];
    uint16_t writeData;
    uint16_t readData;
    SPI1_STATUS status;
    unsigned int    total;
    SPI1_InitializeSPI;
    total = 0;
    do
    {
        total  = SPI1_Exchange16bitBuffer( &myWriteBuffer[total], MY_BUFFER_SIZE - total, &myWriteBuffer[total]);

        // Do something else...

    } while( total < MY_BUFFER_SIZE );

    readData = SPI1_Exchange16bit( writeData);

    status = SPI1_StatusGet();

    </code>

*/

void SPI1_InitializeSPI (void);

/**
    void DRV_SPI1_InitializeSPI(void)
*/
void DRV_SPI1_InitializeSPI(void) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse SPI1_InitializeSPI instead. ")));




/**
  @Summary
    Exchanges one byte of data from SPI1

  @Description
    This routine exchanges one byte of data from the SPI1.
    This is a blocking routine.

  @Preconditions
    The SPI1_Initialize routine must have been called for the specified
    SPI1 driver instance.

  @Returns
    Data read from SPI1

  @Param
    data         - Data to be written onto SPI1.

  @Example 
    Refer to SPI1_Initialize() for an example	
*/
        
uint8_t SPI1_Exchange8bit( uint8_t data );

/**
    uint8_t DRV_SPI1_Exchange8bit(uint8_t data)
*/
uint8_t DRV_SPI1_Exchange8bit(uint8_t data) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse SPI1_Exchange8bit instead. ")));

/**
  @Summary
    Exchanges data from a buffer of size one byte from SPI1

  @Description
    This routine exchanges data from a buffer of size one byte from the SPI1.
    This is a blocking routine.

  @Preconditions
    The SPI1_Initialize routine must have been called for the specified
    SPI1 driver instance.

  @Returns
    Number of words written/read.

  @Param
    dataTransmitted         - Buffer of data to be written onto SPI1.
 
  @Param
    byteCount         - Number of bytes to be exchanged.
 
  @Param
    dataReceived         - Buffer of data to be read from SPI1.

  @Example 
    Refer to SPI1_Initialize() for an example	
 
*/

uint16_t SPI1_Exchange8bitBuffer(uint8_t *dataTransmitted, uint16_t byteCount, uint8_t *dataReceived);

/**
    uint8_t DRV_SPI1_Exchange8bitBuffer(uint8_t *dataTransmitted, uint8_t byteCount, uint8_t *dataReceived)
*/
uint8_t DRV_SPI1_Exchange8bitBuffer(uint8_t *dataTransmitted, uint16_t byteCount, uint8_t *dataReceived) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse SPI1_Exchange8bitBuffer instead. ")));

/**
  @Summary
    Returns the value of the status register of SPI instance : 1

  @Description
    This routine returns the value of the status register of SPI1 driver instance : 1

  @Preconditions
    None.

  @Returns
    Returns the value of the status register.

  @Param
    None.

  @Example 
    Refer to SPI1_Initialize() for an example	
 
*/

SPI1_STATUS SPI1_StatusGet(void);

/**
    DRV_SPI1_STATUS DRV_SPI1_StatusGet(void)
*/
DRV_SPI1_STATUS DRV_SPI1_StatusGet(void) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse SPI1_StatusGet instead. ")));

#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif //_SPI1_H
    
/*******************************************************************************
 End of File
*/
