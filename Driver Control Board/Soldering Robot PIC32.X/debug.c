/*
 *
 * Debug functions
 *
 * This library deals with debugger communictaion
 *
 * Uses UART 6
 *
 */
#ifdef __XC32
    #include <xc.h>          /* Defines special funciton registers, CP0 regs  */
#endif

#include "system.h"
#include <plib.h>
#include <sys/attribs.h>     /* For __ISR definition                          */
#include <stdint.h>          /* For uint32_t definition                       */
#include <stdbool.h>         /* For true/false definition */

#define DEBUG_MODULE_ID UART6
#define DEBUG_BAUDRATE (115200) //The desired BaudRate

// initialise the Debug port

void initDebugPort(void){
    // Configure UART module, set buad rate, turn on UART, etc.
    UARTConfigure(DEBUG_MODULE_ID, UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetFifoMode(DEBUG_MODULE_ID, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetLineControl(DEBUG_MODULE_ID, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UARTSetDataRate(DEBUG_MODULE_ID, GetPeripheralClock(), DEBUG_BAUDRATE);
    UARTEnable(DEBUG_MODULE_ID, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

    // Configure UART RX Interrupt
    INTEnable(INT_SOURCE_UART_RX(DEBUG_MODULE_ID), INT_ENABLED);
    INTSetVectorPriority(INT_VECTOR_UART(DEBUG_MODULE_ID), INT_PRIORITY_LEVEL_7);
    INTSetVectorSubPriority(INT_VECTOR_UART(DEBUG_MODULE_ID), INT_SUB_PRIORITY_LEVEL_0);

}


// Helper functions
void Trace(const char *string)
{
    while(*string != '\0')
    {
        while(!UARTTransmitterIsReady(DEBUG_MODULE_ID))
            ;

        UARTSendDataByte(DEBUG_MODULE_ID, *string);

        string++;

        while(!UARTTransmissionHasCompleted(DEBUG_MODULE_ID))
            ;
    }
}
void EchoCharacter(const char character)
{
        while(!UARTTransmitterIsReady(DEBUG_MODULE_ID));

        UARTSendDataByte(DEBUG_MODULE_ID, character);

        while(!UARTTransmissionHasCompleted(DEBUG_MODULE_ID));
}

// UART 6 interrupt handler, set at priority level 2

void __ISR(_UART_6_VECTOR, IPL7AUTO) IntUart6Handler(void)
{

        if(INTGetFlag(INT_SOURCE_UART_RX(DEBUG_MODULE_ID)))
	{
            // Clear the RX interrupt Flag
	    INTClearFlag(INT_SOURCE_UART_RX(DEBUG_MODULE_ID));

            // Echo what we just received.
            EchoCharacter(UARTGetDataByte(DEBUG_MODULE_ID));

	}

	// We don't care about TX interrupt
	if ( INTGetFlag(INT_SOURCE_UART_TX(DEBUG_MODULE_ID)) )
	{
            INTClearFlag(INT_SOURCE_UART_TX(DEBUG_MODULE_ID));
	}
}
