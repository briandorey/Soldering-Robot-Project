/**
  UART1 Generated Driver API Header File 

  @Company
    Microchip Technology Inc.

  @File Name
    uart1.h

  @Summary
    This is the generated header file for the UART1 driver using MPLAB® Code Configurator

  @Description
    This header file provides APIs for driver for UART1. 
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

#ifndef _UART1_H
#define _UART1_H

/**
 Section: Included Files
*/

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
        
/**
  Section: Data Types
*/

/** UART1 Driver Hardware Flags

  @Summary
    Specifies the status of the hardware receive or transmit

  @Description
    This type specifies the status of the hardware receive or transmit.
    More than one of these values may be OR'd together to create a complete
    status value.  To test a value of this type, the bit of interest must be
    AND'ed with value and checked to see if the result is non-zero.
*/
typedef enum
{
    /* Indicates that Receive buffer has data, at least one more character can be read */
    UART1_RX_DATA_AVAILABLE
        /*DOM-IGNORE-BEGIN*/  = (1 << 0) /*DOM-IGNORE-END*/,
    
    /* Indicates that Receive buffer has overflowed */
    UART1_RX_OVERRUN_ERROR
        /*DOM-IGNORE-BEGIN*/  = (1 << 1) /*DOM-IGNORE-END*/,

    /* Indicates that Framing error has been detected for the current character */
    UART1_FRAMING_ERROR
        /*DOM-IGNORE-BEGIN*/  = (1 << 2) /*DOM-IGNORE-END*/,

    /* Indicates that Parity error has been detected for the current character */
    UART1_PARITY_ERROR
        /*DOM-IGNORE-BEGIN*/  = (1 << 3) /*DOM-IGNORE-END*/,

    /* Indicates that Receiver is Idle */
    UART1_RECEIVER_IDLE
        /*DOM-IGNORE-BEGIN*/  = (1 << 4) /*DOM-IGNORE-END*/,

    /* Indicates that the last transmission has completed */
    UART1_TX_COMPLETE
        /*DOM-IGNORE-BEGIN*/  = (1 << 8) /*DOM-IGNORE-END*/,

    /* Indicates that Transmit buffer is full */
    UART1_TX_FULL
        /*DOM-IGNORE-BEGIN*/  = (1 << 9) /*DOM-IGNORE-END*/

}UART1_STATUS;

/**
   DRV_UART1 Driver Hardware Flags
*/
typedef enum
{
    /* Indicates that Receive buffer has data, at least one more character can be read */
    DRV_UART1_RX_DATA_AVAILABLE
        /*DOM-IGNORE-BEGIN*/  = (1 << 0) /*DOM-IGNORE-END*/,
    
    /* Indicates that Receive buffer has overflowed */
    DRV_UART1_RX_OVERRUN_ERROR
        /*DOM-IGNORE-BEGIN*/  = (1 << 1) /*DOM-IGNORE-END*/,

    /* Indicates that Framing error has been detected for the current character */
    DRV_UART1_FRAMING_ERROR
        /*DOM-IGNORE-BEGIN*/  = (1 << 2) /*DOM-IGNORE-END*/,

    /* Indicates that Parity error has been detected for the current character */
    DRV_UART1_PARITY_ERROR
        /*DOM-IGNORE-BEGIN*/  = (1 << 3) /*DOM-IGNORE-END*/,

    /* Indicates that Receiver is Idle */
    DRV_UART1_RECEIVER_IDLE
        /*DOM-IGNORE-BEGIN*/  = (1 << 4) /*DOM-IGNORE-END*/,

    /* Indicates that the last transmission has completed */
    DRV_UART1_TX_COMPLETE
        /*DOM-IGNORE-BEGIN*/  = (1 << 8) /*DOM-IGNORE-END*/,

    /* Indicates that Transmit buffer is full */
    DRV_UART1_TX_FULL
        /*DOM-IGNORE-BEGIN*/  = (1 << 9) /*DOM-IGNORE-END*/

}DRV_UART1_STATUS;

/** UART1 Driver Transfer Flags

  @Summary
    Specifies the status of the receive or transmit

  @Description
    This type specifies the status of the receive or transmit operation.
    More than one of these values may be OR'd together to create a complete
    status value.  To test a value of this type, the bit of interest must be
    AND'ed with value and checked to see if the result is non-zero.
*/

typedef enum
{
    /* Indicates that the core driver buffer is full */
    UART1_TRANSFER_STATUS_RX_FULL
        /*DOM-IGNORE-BEGIN*/  = (1 << 0) /*DOM-IGNORE-END*/,

    /* Indicates that at least one byte of Data has been received */
    UART1_TRANSFER_STATUS_RX_DATA_PRESENT
        /*DOM-IGNORE-BEGIN*/  = (1 << 1) /*DOM-IGNORE-END*/,

    /* Indicates that the core driver receiver buffer is empty */
    UART1_TRANSFER_STATUS_RX_EMPTY
        /*DOM-IGNORE-BEGIN*/  = (1 << 2) /*DOM-IGNORE-END*/,

    /* Indicates that the core driver transmitter buffer is full */
    UART1_TRANSFER_STATUS_TX_FULL
        /*DOM-IGNORE-BEGIN*/  = (1 << 3) /*DOM-IGNORE-END*/,

    /* Indicates that the core driver transmitter buffer is empty */
    UART1_TRANSFER_STATUS_TX_EMPTY
        /*DOM-IGNORE-BEGIN*/  = (1 << 4) /*DOM-IGNORE-END*/

} UART1_TRANSFER_STATUS;

/** 
    DRV_UART1 Driver Transfer Flags
*/
typedef enum
{
    /* Indicates that the core driver buffer is full */
    DRV_UART1_TRANSFER_STATUS_RX_FULL
        /*DOM-IGNORE-BEGIN*/  = (1 << 0) /*DOM-IGNORE-END*/,

    /* Indicates that at least one byte of Data has been received */
    DRV_UART1_TRANSFER_STATUS_RX_DATA_PRESENT
        /*DOM-IGNORE-BEGIN*/  = (1 << 1) /*DOM-IGNORE-END*/,

    /* Indicates that the core driver receiver buffer is empty */
    DRV_UART1_TRANSFER_STATUS_RX_EMPTY
        /*DOM-IGNORE-BEGIN*/  = (1 << 2) /*DOM-IGNORE-END*/,

    /* Indicates that the core driver transmitter buffer is full */
    DRV_UART1_TRANSFER_STATUS_TX_FULL
        /*DOM-IGNORE-BEGIN*/  = (1 << 3) /*DOM-IGNORE-END*/,

    /* Indicates that the core driver transmitter buffer is empty */
    DRV_UART1_TRANSFER_STATUS_TX_EMPTY
        /*DOM-IGNORE-BEGIN*/  = (1 << 4) /*DOM-IGNORE-END*/

} DRV_UART1_TRANSFER_STATUS;

/**
  Section: UART1 Driver Routines
*/


/**
  @Summary
    Initializes the UART instance : 1

  @Description
    This routine initializes the UART driver instance for : 1
    index.
    This routine must be called before any other UART routine is called.
    
  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Comment
    RS485 Communication Port
 
  @Example
    <code>
        const uint8_t writeBuffer[35] = "1234567890ABCDEFGHIJKLMNOP\n" ;
        unsigned int numBytes = 0;
        int  writebufferLen = strlen((char *)writeBuffer);
        UART1_InitializeRS485();
        while(numBytes < writebufferLen)
        {    
            int bytesToWrite = UART1_TransmitBufferSizeGet();
            numBytes = UART1_WriteBuffer ( writeBuffer+numBytes, bytesToWrite)  ;
            UART1_TasksTransmit ( );
            if (!UART1_TransmitBufferisFull())
            {
                //continue other operation
            }
        }
    </code>

*/

void UART1_InitializeRS485(void);
/**
    void DRV_UART1_InitializeRS485(void)
*/
void DRV_UART1_InitializeRS485(void) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse UART1_InitializeRS485 instead. ")));





/**
  @Summary
    Read a byte of data from the UART1

  @Description
    This routine reads a byte of data from the UART1.

  @Preconditions
    UART1_Initializer function should have been called 
    before calling this function. The transfer status should be checked to see 
    if the receiver is not empty before calling this function.

  @Param
    None.

  @Returns
    A data byte received by the driver.

  @Example
    <code>
    char            myBuffer[MY_BUFFER_SIZE];
    unsigned int    numBytes;

    numBytes = 0;
    do
    {
        if( UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet() )
        {
            myBuffer[numBytes++] = UART1_Read();
        }

        // Do something else...

    } while( numBytes < MY_BUFFER_SIZE);
    </code>
*/

uint8_t UART1_Read( void);
/**
    uint8_t DRV_UART1_Read( void)
*/
uint8_t DRV_UART1_Read( void) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse UART1_Read instead. ")));

/**
  @Summary
    Returns the number of bytes read by the UART1 peripheral

  @Description
    This routine returns the number of bytes read by the Peripheral and fills the
    application read buffer with the read data.

  @Preconditions
    UART1_Initializer function should have been called 
    before calling this function

  @Param
    buffer       - Buffer into which the data read from the UART1

  @Param
    numbytes     - Total number of bytes that need to be read from the UART1
                   (must be equal to or less than the size of the buffer)

  @Returns
    Number of bytes actually copied into the caller's buffer or -1 if there
    is an error.

  @Example
    <code>
    char                     myBuffer[MY_BUFFER_SIZE];
    unsigned int             numBytes;
    UART1_TRANSFER_STATUS status ;

    // Pre-initialize myBuffer with MY_BUFFER_SIZE bytes of valid data.

    numBytes = 0;
    while( numBytes < MY_BUFFER_SIZE);
    {
        status = UART1_TransferStatusGet ( ) ;
        if (status & UART1_TRANSFER_STATUS_RX_FULL)
        {
            numBytes += UART1_ReadBuffer( myBuffer + numBytes, MY_BUFFER_SIZE - numBytes )  ;
            if(numBytes < readbufferLen)
            {
                continue;
            }
            else
            {
                break;
            }
        }
        else
        {
            continue;
        }

        // Do something else...
    }
    </code>
*/

unsigned int UART1_ReadBuffer( uint8_t *buffer ,  const unsigned int numbytes);
/**
    unsigned int DRV_UART1_ReadBuffer( uint8_t *buffer , const unsigned int numbytes)
*/
unsigned int DRV_UART1_ReadBuffer( uint8_t *buffer ,const unsigned int numbytes) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse UART1_ReadBuffer instead. ")));

/**
  @Summary
    Writes a byte of data to the UART1

  @Description
    This routine writes a byte of data to the UART1.

  @Preconditions
    UART1_Initializer function should have been called 
    before calling this function. The transfer status should be checked to see if
    transmitter is not full before calling this function.

  @Param
    byte         - Data byte to write to the UART1

  @Returns
    None.

  @Example
    <code>
    char            myBuffer[MY_BUFFER_SIZE];
    unsigned int    numBytes;

    // Pre-initialize myBuffer with MY_BUFFER_SIZE bytes of valid data.

    numBytes = 0;
    while( numBytes < MY_BUFFER_SIZE);
    {
        if( !(UART1_TRANSFER_STATUS_TX_FULL & UART1_TransferStatusGet()) )
        {
            UART1_Write(handle, myBuffer[numBytes++]);
        }

        // Do something else...
    }
    </code>
*/

void UART1_Write( const uint8_t byte);
/**
    void DRV_UART1_Write( const uint8_t byte)
*/
void DRV_UART1_Write( const uint8_t byte) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse UART1_Write instead. ")));


/**
  @Summary
    Returns the number of bytes written into the internal buffer

  @Description
    This API transfers the data from application buffer to internal buffer and 
    returns the number of bytes added in that queue

  @Preconditions
    UART1_Initializer function should have been called 
    before calling this function

  @Example
    <code>
    char                     myBuffer[MY_BUFFER_SIZE];
    unsigned int             numBytes;
    UART1_TRANSFER_STATUS status ;

    // Pre-initialize myBuffer with MY_BUFFER_SIZE bytes of valid data.

    numBytes = 0;
    while( numBytes < MY_BUFFER_SIZE);
    {
        status = UART1_TransferStatusGet ( ) ;
        if (status & UART1_TRANSFER_STATUS_TX_EMPTY)
        {
            numBytes += UART1_WriteBuffer ( myBuffer + numBytes, MY_BUFFER_SIZE - numBytes )  ;
            if(numBytes < writebufferLen)
            {
                continue;
            }
            else
            {
                break;
            }
        }
        else
        {
            continue;
        }

        // Do something else...
    }
    </code>
*/

unsigned int UART1_WriteBuffer( const uint8_t *buffer , const unsigned int numbytes );
/**
    unsigned int DRV_UART1_WriteBuffer( const uint8_t *buffer , const unsigned int numbytes )
*/
unsigned int DRV_UART1_WriteBuffer( const uint8_t *buffer , const unsigned int numbytes ) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse UART1_WriteBuffer instead. ")));


/**
  @Summary
    Returns the transmitter and receiver transfer status

  @Description
    This returns the transmitter and receiver transfer status.The returned status 
    may contain a value with more than one of the bits
    specified in the UART1_TRANSFER_STATUS enumeration set.  
    The caller should perform an "AND" with the bit of interest and verify if the
    result is non-zero (as shown in the example) to verify the desired status
    bit.

  @Preconditions
    UART1_Initializer function should have been called 
    before calling this function

  @Param
    None.

  @Returns
    A UART1_TRANSFER_STATUS value describing the current status 
    of the transfer.

  @Example
    Refer to UART1_ReadBuffer and UART1_WriteBuffer for example

*/

UART1_TRANSFER_STATUS UART1_TransferStatusGet (void );
/**
    DRV_UART1_TRANSFER_STATUS DRV_UART1_TransferStatusGet (void )
*/
DRV_UART1_TRANSFER_STATUS DRV_UART1_TransferStatusGet (void ) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse UART1_TransferStatusGet instead. ")));



/**
  @Summary
    Returns the character in the read sequence at the offset provided, without
    extracting it

  @Description
    This routine returns the character in the read sequence at the offset provided,
    without extracting it
 
  @Param
    None.
    
  @Example 
    <code>
    const uint8_t readBuffer[5];
    unsigned int data, numBytes = 0;
    unsigned int readbufferLen = sizeof(readBuffer);
    UART1_Initializer();
    
    while(numBytes < readbufferLen)        
    {   
        UART1_TasksReceive ( );
        //Check for data at a particular place in the buffer
        data = UART1_Peek(3);
        if(data == 5)
        {
            //discard all other data if byte that is wanted is received.    
            //continue other operation
            numBytes += UART1_ReadBuffer ( readBuffer + numBytes , readbufferLen ) ;
        }
        else
        {
            break;
        }
    }
    </code>
 
*/
uint8_t UART1_Peek(uint16_t offset);
/**
    uint8_t DRV_UART1_Peek(uint16_t offset)
*/
uint8_t DRV_UART1_Peek(uint16_t offset) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse UART1_Peek instead. ")));


/**
  @Summary
    Returns the size of the receive buffer

  @Description
    This routine returns the size of the receive buffer.

  @Param
    None.

  @Returns
    Size of receive buffer.
    
  @Example 
    <code>
    const uint8_t readBuffer[5];
    unsigned int size, numBytes = 0;
    unsigned int readbufferLen = sizeof(readBuffer);
    UART1__Initializer();
    
    while(size < readbufferLen)
	{
	    UART1_TasksReceive ( );
	    size = UART1_ReceiveBufferSizeGet();
	}
    numBytes = UART1_ReadBuffer ( readBuffer , readbufferLen ) ;
    </code>
 
*/
unsigned int UART1_ReceiveBufferSizeGet(void);
/**
    unsigned int DRV_UART1_ReceiveBufferSizeGet(void)
*/
unsigned int DRV_UART1_ReceiveBufferSizeGet(void) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse UART1_ReceiveBufferSizeGet instead. ")));

/**
  @Summary
    Returns the size of the transmit buffer

  @Description
    This routine returns the size of the transmit buffer.

 @Param
    None.
 
 @Returns
    Size of transmit buffer.

 @Example
    Refer to UART1_Initializer(); for example.
*/
unsigned int UART1_TransmitBufferSizeGet(void);
/**
    unsigned int DRV_UART1_TransmitBufferSizeGet(void)
*/
unsigned int DRV_UART1_TransmitBufferSizeGet(void) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse UART1_TransmitBufferSizeGet instead. ")));


/**
  @Summary
    Returns the status of the receive buffer

  @Description
    This routine returns if the receive buffer is empty or not.

  @Param
    None.
 
  @Returns
    True if the receive buffer is empty
    False if the receive buffer is not empty
    
  @Example
    <code>
    char                     myBuffer[MY_BUFFER_SIZE];
    unsigned int             numBytes;
    UART1_TRANSFER_STATUS status ;

    // Pre-initialize myBuffer with MY_BUFFER_SIZE bytes of valid data.

    numBytes = 0;
    while( numBytes < MY_BUFFER_SIZE);
    {
        status = UART1_TransferStatusGet ( ) ;
        if (!UART1_ReceiveBufferIsEmpty())
        {
            numBytes += UART1_ReadBuffer( myBuffer + numBytes, MY_BUFFER_SIZE - numBytes )  ;
            if(numBytes < readbufferLen)
            {
                continue;
            }
            else
            {
                break;
            }
        }
        else
        {
            continue;
        }

        // Do something else...
    }
    </code>
 
*/
bool UART1_ReceiveBufferIsEmpty (void);
/**
    bool DRV_UART1_ReceiveBufferIsEmpty (void)
*/
bool DRV_UART1_ReceiveBufferIsEmpty (void) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse UART1_ReceiveBufferIsEmpty instead. ")));


/**
  @Summary
    Returns the status of the transmit buffer

  @Description
    This routine returns if the transmit buffer is full or not.

 @Param
    None.
 
 @Returns
    True if the transmit buffer is full
    False if the transmit buffer is not full

 @Example
    Refer to UART1_Initializer() for example.
 
*/
bool UART1_TransmitBufferIsFull (void);
/**
    bool DRV_UART1_TransmitBufferIsFull (void)
*/
bool DRV_UART1_TransmitBufferIsFull (void) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse UART1_TransmitBufferIsFull instead. ")));


/**
  @Summary
    Returns the transmitter and receiver status

  @Description
    This returns the transmitter and receiver status. The returned status may 
    contain a value with more than one of the bits
    specified in the UART1_STATUS enumeration set.  
    The caller should perform an "AND" with the bit of interest and verify if the
    result is non-zero (as shown in the example) to verify the desired status
    bit.

  @Preconditions
    UART1_Initializer function should have been called 
    before calling this function

  @Param
    None.

  @Returns
    A UART1_STATUS value describing the current status 
    of the transfer.

  @Example
    <code>
        while(!(UART1_StatusGet & UART1_TX_COMPLETE ))
        {
           // Wait for the tranmission to complete
        }
    </code>
*/

UART1_STATUS UART1_StatusGet (void );
/**
    DRV_UART1_STATUS UART1_StatusGet (void )
*/
DRV_UART1_STATUS DRV_UART1_StatusGet (void ) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse UART1_StatusGet instead. ")));

void RS485_WriteString(const uint8_t *writeBuffer);
void WaitForNewLine(void);

#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
    
#endif  // _UART1_H

/*
  End of File
*/

