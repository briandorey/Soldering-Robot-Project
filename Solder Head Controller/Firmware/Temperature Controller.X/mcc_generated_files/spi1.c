
/**
  SPI1 Generated Driver API Source File

  Company:
    Microchip Technology Inc.

  File Name:
    spi1.c

  @Summary
    This is the generated source file for the SPI1 driver using MPLAB® Code Configurator

  @Description
    This source file provides APIs for driver for SPI1.
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

/**
  Section: Included Files
*/

#include <xc.h>
#include "spi1.h"


/**
 Section: File specific functions
*/

inline __attribute__((__always_inline__)) SPI1_TRANSFER_MODE SPI1_TransferModeGet(void);
void SPI1_Exchange( uint8_t *pTransmitData, uint8_t *pReceiveData );
uint16_t SPI1_ExchangeBuffer(uint8_t *pTransmitData, uint16_t byteCount, uint8_t *pReceiveData);

/**
 Section: Driver Interface Function Definitions
*/


void SPI1_InitializeSPI (void)
{
    // SPI Frequency = 35,714 Hz; SSEN disabled; PPRE 64:1; SMP Sample at Middle; MSTEN enabled; MODE16 disabled; DISSCK disabled; SPRE 7:1; CKE Idle to Active; DISSDO disabled; CKP Idle:Low, Active:High; 
    SPI1CON1 = 0x0024;
    // SPIFSD disabled; FRMPOL disabled; SPIBEN disabled; FRMDLY disabled; FRMEN disabled; 
    SPI1CON2 = 0x0000;
    // SISEL SPI_INT_LAST_WORD_READ; SPIEN enabled; SPISIDL disabled; SPIROV disabled; 
    SPI1STAT = 0x8000;

}

/**
    void DRV_SPI1_InitializeSPI (void)
*/
void DRV_SPI1_InitializeSPI (void)
{
    SPI1_InitializeSPI ();
}


void SPI1_Exchange( uint8_t *pTransmitData, uint8_t *pReceiveData )
{

    while( SPI1STATbits.SPITBF == true )
    {

    }

    if (SPI1_TransferModeGet() == SPI1_DRIVER_TRANSFER_MODE_16BIT)
        SPI1BUF = *((uint16_t*)pTransmitData);
    else
        SPI1BUF = *((uint8_t*)pTransmitData);

    while ( SPI1STATbits.SRXMPT == true);


    if (SPI1_TransferModeGet() == SPI1_DRIVER_TRANSFER_MODE_16BIT)
        *((uint16_t*)pReceiveData) = SPI1BUF;
    else
        *((uint8_t*)pReceiveData) = SPI1BUF;

}

uint16_t SPI1_ExchangeBuffer(uint8_t *pTransmitData, uint16_t byteCount, uint8_t *pReceiveData)
{

    uint16_t dataSentCount = 0;
    uint16_t count = 0;
    uint16_t dummyDataReceived = 0;
    uint16_t dummyDataTransmit = SPI1_DUMMY_DATA;

    uint8_t  *pSend, *pReceived;
    uint16_t addressIncrement;
    uint16_t receiveAddressIncrement, sendAddressIncrement;

    SPI1_TRANSFER_MODE spiModeStatus;

    spiModeStatus = SPI1_TransferModeGet();
    // set up the address increment variable
    if (spiModeStatus == SPI1_DRIVER_TRANSFER_MODE_16BIT)
    {
        addressIncrement = 2;
        byteCount >>= 1;
    }        
    else
    {
        addressIncrement = 1;
    }

    // set the pointers and increment delta 
    // for transmit and receive operations
    if (pTransmitData == NULL)
    {
        sendAddressIncrement = 0;
        pSend = (uint8_t*)&dummyDataTransmit;
    }
    else
    {
        sendAddressIncrement = addressIncrement;
        pSend = (uint8_t*)pTransmitData;
    }
        
    if (pReceiveData == NULL)
    {
       receiveAddressIncrement = 0;
       pReceived = (uint8_t*)&dummyDataReceived;
    }
    else
    {
       receiveAddressIncrement = addressIncrement;        
       pReceived = (uint8_t*)pReceiveData;
    }


    while( SPI1STATbits.SPITBF == true )
    {

    }

    while (dataSentCount < byteCount)
    {
        if ((count < SPI1_FIFO_FILL_LIMIT))
        {
            if (spiModeStatus == SPI1_DRIVER_TRANSFER_MODE_16BIT)
                SPI1BUF = *((uint16_t*)pSend);
            else
                SPI1BUF = *pSend;
            pSend += sendAddressIncrement;
            dataSentCount++;
            count++;
        }

        if (SPI1STATbits.SRXMPT == false)
        {
            if (spiModeStatus == SPI1_DRIVER_TRANSFER_MODE_16BIT)
                *((uint16_t*)pReceived) = SPI1BUF;
            else
                *pReceived = SPI1BUF;
            pReceived += receiveAddressIncrement;
            count--;
        }

    }
    while (count)
    {
        if (SPI1STATbits.SRXMPT == false)
        {
            if (spiModeStatus == SPI1_DRIVER_TRANSFER_MODE_16BIT)
                *((uint16_t*)pReceived) = SPI1BUF;
            else
                *pReceived = SPI1BUF;
            pReceived += receiveAddressIncrement;
            count--;
        }
    }

    return dataSentCount;
}



uint8_t SPI1_Exchange8bit( uint8_t data )
{
    uint8_t receiveData;
    
    SPI1_Exchange(&data, &receiveData);

    return (receiveData);
}

/**
    uint8_t DRV_SPI1_Exchange16bit (uint8_t data)
*/
uint8_t DRV_SPI1_Exchange8bit (uint8_t data)
{
    return(SPI1_Exchange8bit (data));
}

uint16_t SPI1_Exchange8bitBuffer(uint8_t *dataTransmitted, uint16_t byteCount, uint8_t *dataReceived)
{
    return (SPI1_ExchangeBuffer(dataTransmitted, byteCount, dataReceived));
}

/**
    uint8_t DRV_SPI1_Exchange8bitBuffer (uint8_t *dataTransmitted, uint8_t byteCount, uint8_t *dataReceived)
*/
uint8_t DRV_SPI1_Exchange8bitBuffer (uint8_t *dataTransmitted, uint16_t byteCount, uint8_t *dataReceived)
{
    return(SPI1_Exchange8bitBuffer ((uint8_t *)dataTransmitted, byteCount, (uint8_t *)dataReceived ));
}


inline __attribute__((__always_inline__)) SPI1_TRANSFER_MODE SPI1_TransferModeGet(void)
{
	if (SPI1CON1bits.MODE16 == 0)
        return SPI1_DRIVER_TRANSFER_MODE_8BIT;
    else
        return SPI1_DRIVER_TRANSFER_MODE_16BIT;
}


SPI1_STATUS SPI1_StatusGet()
{
    return(SPI1STAT);
}

/**
    DRV_SPI1_STATUS DRV_SPI1_StatusGet ()
*/
DRV_SPI1_STATUS DRV_SPI1_StatusGet ()
{
    return(SPI1_StatusGet ());
}
/**
 End of File
*/
