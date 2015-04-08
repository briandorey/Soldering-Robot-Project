/*
 *
 * RS485 functions
 *
 * This library deals with RS485 communictaion to the soldering iron temperature controller
 *
 * Uses UART 3
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

#define RS485_MODULE_ID UART3
#define RS485_BAUDRATE (115200) //The desired BaudRate

/* RS485 Command Protocol
     *   G = Get Status, returns Iron status and temperature for both irons. 20 bytes
     *   Format G[Iron1 Status, 1 byte],[Iron2 Status, 1 byte],[Iron1 Temperature, 6 bytes],[Iron1 Temperature, 6 bytes]   example G,1,1,253.22,433.22/n
     *
     *   S1 = Read Soldering Iron 1 status (S1,0 = off, S1,1 = on)
     *   S2 = Read Soldering Iron 2 status (S2,0 = off, S2,1 = on)
     *   A,1 = Set Both Active (0 = off, 1 = on)
     *   I1,1 = Set Soldering Iron 1 Active (0 = off, 1 = on)
     *   I2,1 = Set Soldering Iron 2 Active (0 = off, 1 = on)
     *   R1 = Read Soldering Iron 1 Temperature (float 3.2 digits) Returns "R1,100.00"
     *   R2 = Read Soldering Iron 2 Temperature (float 3.2 digits) Returns "R2,100.00"
     *   T1,380.00 = Set Soldering Iron 1 Target Temperature (float 3.2 digits)
     *   T2,380.00 = Set Soldering Iron 2 Target Temperature (float 3.2 digits)
     *
     *   All commands end with a \n character
    */

bool UART_NEWLINE_RECEIVED = false;
char UART_RECEIVED_STRING[44];
unsigned char UART_CURRENT_BYTE = 0;
int UART_CURRENT_POSITION = 0;
int UART_TIMEOUT = 5000;

// soldering iron variables

unsigned char SolderingIron1Status;
unsigned char SolderingIron2Status;
unsigned char SolderingIron1Activate;
unsigned char SolderingIron2Activate;
unsigned char SolderingIron1Temperature[6];
unsigned char SolderingIron2Temperature[6];
unsigned char SolderingIron1TargetTemperature[6];
unsigned char SolderingIron2TargetTemperature[6];

// initialise the RS485 port

void initRS485(void){
    // Configure UART module, set buad rate, turn on UART, etc.
    UARTConfigure(RS485_MODULE_ID, UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetFifoMode(RS485_MODULE_ID, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetLineControl(RS485_MODULE_ID, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UARTSetDataRate(RS485_MODULE_ID, GetPeripheralClock(), RS485_BAUDRATE);
    UARTEnable(RS485_MODULE_ID, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

    

    // Configure UART RX Interrupt
    INTEnable(INT_SOURCE_UART_RX(RS485_MODULE_ID), INT_ENABLED);
    INTSetVectorPriority(INT_VECTOR_UART(RS485_MODULE_ID), INT_PRIORITY_LEVEL_5);
    INTSetVectorSubPriority(INT_VECTOR_UART(RS485_MODULE_ID), INT_SUB_PRIORITY_LEVEL_0);

    INTClearFlag(INT_U3RX);

    // Configure Timer4/5 to send a get status command every 500 ms

    OpenTimer45(T4_ON | T45_SOURCE_INT | T4_32BIT_MODE_ON | T45_PS_1_64, 0x0007270e);
    ConfigIntTimer45(T45_INT_ON | T45_INT_PRIOR_5 | T45_INT_SUB_PRIOR_1);
    INTClearFlag(INT_T5);
}



void RS485ReceivedCommand(char RECEIVED[44]){
    // get the status and temperatures for the soldering irons
    //Format G[Iron1 Status, 1 byte],[Iron2 Status, 1 byte],[Iron1 Temperature, 6 bytes],[Iron1 Temperature, 6 bytes]   example G,1,1,253.22,433.22/n
    if (RECEIVED[0] == 'G'){ // Iron 1 Status command received
        if (RECEIVED[2] == '1'){ // Iron 1 is switched on
            SolderingIron1Status = 1;
        }
        if (RECEIVED[2] == '0'){ // Iron 1 is switched on
            SolderingIron1Status = 0;
        }
        if (RECEIVED[4] == '1'){ // Iron 1 is switched on
            SolderingIron2Status = 1;
        }
        if (RECEIVED[4] == '0'){ // Iron 1 is switched on
            SolderingIron2Status = 0;
        }
        SolderingIron1Temperature[0] = RECEIVED[6];
        SolderingIron1Temperature[1] = RECEIVED[7];
        SolderingIron1Temperature[2] = RECEIVED[8];
        SolderingIron1Temperature[3] = RECEIVED[9];
        SolderingIron1Temperature[4] = RECEIVED[10];
        SolderingIron1Temperature[5] = RECEIVED[11];

        SolderingIron2Temperature[0] = RECEIVED[13];
        SolderingIron2Temperature[1] = RECEIVED[14];
        SolderingIron2Temperature[2] = RECEIVED[15];
        SolderingIron2Temperature[3] = RECEIVED[16];
        SolderingIron2Temperature[4] = RECEIVED[17];
        SolderingIron2Temperature[5] = RECEIVED[18];

        SolderingIron1TargetTemperature[0] = RECEIVED[20];
        SolderingIron1TargetTemperature[1] = RECEIVED[21];
        SolderingIron1TargetTemperature[2] = RECEIVED[22];
        SolderingIron1TargetTemperature[3] = RECEIVED[23];
        SolderingIron1TargetTemperature[4] = RECEIVED[24];
        SolderingIron1TargetTemperature[5] = RECEIVED[25];

        SolderingIron2TargetTemperature[0] = RECEIVED[27];
        SolderingIron2TargetTemperature[1] = RECEIVED[28];
        SolderingIron2TargetTemperature[2] = RECEIVED[29];
        SolderingIron2TargetTemperature[3] = RECEIVED[30];
        SolderingIron2TargetTemperature[4] = RECEIVED[31];
        SolderingIron2TargetTemperature[5] = RECEIVED[32];

        if (RECEIVED[34] == '1'){ // Iron 1 is switched on
            SolderingIron1Activate = 1;
        }
        if (RECEIVED[34] == '0'){ // Iron 1 is switched on
            SolderingIron1Activate = 0;
        }
        if (RECEIVED[35] == '1'){ // Iron 1 is switched on
            SolderingIron2Activate = 1;
        }
        if (RECEIVED[35] == '0'){ // Iron 1 is switched on
            SolderingIron2Activate = 0;
        }

    }

    //Trace((char *)RECEIVED);
    //Trace('/r');
}

// Helper functions
void RS485WriteString(const char *string)
{
    while(*string != '\0')
    {
        while(!UARTTransmitterIsReady(RS485_MODULE_ID))
            ;

        UARTSendDataByte(RS485_MODULE_ID, *string);

        string++;

        while(!UARTTransmissionHasCompleted(RS485_MODULE_ID))
            ;
    }
}
void PutCharacter(const char character)
{
        while(!UARTTransmitterIsReady(RS485_MODULE_ID));

        UARTSendDataByte(RS485_MODULE_ID, character);

        while(!UARTTransmissionHasCompleted(RS485_MODULE_ID));
}

// UART 2 interrupt handler, set at priority level 2
void __ISR(_UART_3_VECTOR, IPL5AUTO) IntUart3Handler(void)
{
    //Trace("uart interrupt triggered\n\r");
	// Is this an RX interrupt?
	if(INTGetFlag(INT_SOURCE_UART_RX(RS485_MODULE_ID)))
	{
            

            UART_CURRENT_BYTE = UARTGetDataByte(RS485_MODULE_ID);

            UART_RECEIVED_STRING[UART_CURRENT_POSITION] = UART_CURRENT_BYTE;

            UART_CURRENT_POSITION++;


            // check for new line character

            if (UART_CURRENT_BYTE == '\n' ){
                //UART_NEWLINE_RECEIVED = true;
                UART_CURRENT_POSITION = 0;
                RS485ReceivedCommand(UART_RECEIVED_STRING);
            }
            
            INTClearFlag(INT_U3RX);

	}

	// We don't care about TX interrupt
	if ( INTGetFlag(INT_SOURCE_UART_TX(RS485_MODULE_ID)) )
	{
            INTClearFlag(INT_SOURCE_UART_TX(RS485_MODULE_ID));
	}
}



void setBothIronsActive(bool status){
    // A,1 = Set Both Active (0 = off, 1 = on)
    if (status == true){
        RS485WriteString("A,1\n");
    }
    else{
        RS485WriteString("A,0\n");
    }
}

void setIron1Active(bool status){
    // I1,1 = Set Soldering Iron 1 Active (0 = off, 1 = on)
    if (status == true){
        RS485WriteString("I1,1\n");
    }
    else{
        RS485WriteString("I1,0\n");
    }
}

void setIron2Active(bool status){
    // I2,1 = Set Soldering Iron 2 Active (0 = off, 1 = on)
    if (status == true){
        RS485WriteString("I2,1\n");
    }
    else{
        RS485WriteString("I2,0\n");
    }
}

void setIron1Temperature(char *temperature){
    // T1,380.00 = Set Soldering Iron 1 Target Temperature (float 3.2 digits)
    RS485WriteString("T1,");
    RS485WriteString(temperature);
    RS485WriteString("\n");
}

void setIron2Temperature(char *temperature){
    // T2,380.00 = Set Soldering Iron 2 Target Temperature (float 3.2 digits)
    RS485WriteString("T2,");
    RS485WriteString(temperature);
    RS485WriteString("\n");
}


void __ISR(_TIMER_5_VECTOR,IPL5AUTO) IntTimer5Handler(void){
    INTClearFlag(INT_T5);
    RS485WriteString("G\n");
}