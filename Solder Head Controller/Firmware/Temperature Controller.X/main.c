/**
  Main Source File

  File Name:
    main.c

  Summary:
    This is the main application file

*/


//#define debug
#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/uart1.h" // RS485

#ifdef debug
#include "mcc_generated_files/uart2.h" // Debug Port
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libpic30.h>

// constants
const float MaxTemperature = 500; // the maximum temperature that the controller will allow

// Variables
float SolderHead_1_Temperature; // current temperature for soldering head 1
float SolderHead_2_Temperature; // current temperature for soldering head 2
float SolderHead_1_Target; // target temperature for  for soldering head 1
float SolderHead_2_Target; // target temperature for  for soldering head 2
unsigned char SolderHead_1_Activate = '1'; // stores whether or not soldering head 1 can be turned on
unsigned char SolderHead_2_Activate = '1'; // stores whether or not soldering head 2 can be turned on

float calculated_voltage; // the voltage returned from the ADC input
float calculated_temperature; // the temperature based on the ADC voltage
float fResult = 50000; // tempory value used in the ADC conversion
char status[40]; // char array used for the RS485 bulk transfer
char tempstring[6]; // char array used for tempory conversions
char str_num[20]; // char array used for float to char conversions

/*

 * Global Functions

 */
#ifdef debug

// Trace - used to write traced strings to the debug uart port

void Trace(const uint8_t *writeBuffer){
    int  writebufferLen = strlen((char *)writeBuffer);

     UART2_WriteBuffer ( writeBuffer, writebufferLen)  ;
     while(!(UART2_StatusGet() & UART2_TX_COMPLETE ))
        {
           // Wait for the tranmission to complete
        }
     UART2_Write('\r');
     UART2_Write('\n');
     while(!(UART2_StatusGet() & UART2_TX_COMPLETE ))
        {
           // Wait for the tranmission to complete
        }
}
#endif

// floatToChar - converts a float value to a 6 byte char array 000.00 formatted

char *floatToChar(float val){
    sprintf(str_num , "%06.2f", (double)val );
    return str_num;
}

// charToFloat - converts a 6 byte 000.00 formatted char array to a float

float charToFloat(char *val){
    float f;
    sscanf(val,"%f",&f);
    return f;
}


/*

 * Temperature Control Fuctions

 */

// getTemperatureDefaults - used to set the default values for the soldering head target temperatures

void getTemperatureDefaults(void){
    SolderHead_1_Target = 10;
    SolderHead_2_Target = 10;
}

// calculateTemperature - converts the raw voltage value from the ADC into a temperature based on the thermistor gradient

float calculateTemperature(float rawval){
    calculated_voltage = (0.0000152587890625 * rawval) * 4.094; // multiply the raw adc value by the voltage reference
    calculated_temperature = (calculated_voltage * 406.88) - 230; // convert input voltage to temperature
    return calculated_temperature;
}


// updateTemperatures - compares the soldering iron temperatures against the target temperatures and actiavtes the irons if needed

void updateTemperatures(void){    

    bool bConversionComplete = false;

    // get adc values for both soldering irons
    SDADC1_InitializeADC0();
    while (1)
    {
	bConversionComplete = SDADC1_IsConversionComplete();
        if(true == bConversionComplete)
        {
            fResult = SDADC1_ConversionResultGet();
            break;
        }
    }

    SolderHead_1_Temperature = calculateTemperature(fResult); // calculate the temperature in °C for solder head 1

    bConversionComplete = false;
    SDADC1_InitializeADC1();
    while (1)
    {
	bConversionComplete = SDADC1_IsConversionComplete();
        if(true == bConversionComplete)
        {
            fResult = SDADC1_ConversionResultGet();
            break;
        }
    }
    
    SolderHead_2_Temperature = calculateTemperature(fResult); // calculate the temperature in °C for solder head 2

    #ifdef debug
        Trace((unsigned char *)"Head 1:");
        Trace((unsigned char *)floatToChar(SolderHead_1_Temperature));
       Trace((unsigned char *)"Head 2:");
        Trace((unsigned char *)floatToChar(SolderHead_2_Temperature));
    #endif

    if (SolderHead_1_Activate == '1'){
        if ((SolderHead_1_Temperature < SolderHead_1_Target) && (SolderHead_1_Temperature < MaxTemperature))
        { // switch solder head 1 on if the temperature is too low
            SolderHead_1_SetHigh();
            SolderHead_LED1_SetHigh();
        }
        else{
            SolderHead_1_SetLow();
            SolderHead_LED1_SetLow();
        }
    }
    else{
            SolderHead_1_SetLow();
            SolderHead_LED1_SetLow();
    }

    if (SolderHead_2_Activate == '1'){
        if ((SolderHead_2_Temperature < SolderHead_2_Target) && (SolderHead_2_Temperature < MaxTemperature))
        { // switch solder head 2 on if the temperature is too low
            SolderHead_2_SetHigh();
            SolderHead_LED2_SetHigh();
        }
        else{
            SolderHead_2_SetLow();
            SolderHead_LED2_SetLow();
        }
    }
    else{
            SolderHead_2_SetLow();
            SolderHead_LED2_SetLow();
    }

    // Reset the watchdog
    ClrWdt();
    
}

/*
 * 
 *  Uart Functions
 *
 */

void RS485Tasks(int uart_port){

    /* RS485 Command Protocol
     *   G = Get Status, returns Iron status and temperature for both irons. 35 bytes
     *   Format G[Iron1 Status, 1 byte],[Iron2 Status, 1 byte],[Iron1 Temperature, 6 bytes],[Iron2 Temperature, 6 bytes],
     *           [Iron1 Taget Temperature, 6 bytes],[Iron2 Target Temperature, 6 bytes], [Iron1 Activate status][Iron2 Activate status]  example G,1,1,253.22,433.22/n
     *
     *   S1 = Read Soldering Iron 1 status (0 = off, 1 = on)
     *   S2 = Read Soldering Iron 2 status (0 = off, 1 = on)
     *   A,1 = Set Both Activate (0 = off, 1 = on)
     *   I1,1 = Set Soldering Iron 1 Activate (0 = off, 1 = on)
     *   I2,1 = Set Soldering Iron 2 Activate (0 = off, 1 = on)
     *   R1 = Read Soldering Iron 1 Temperature (float 3.2 digits)
     *   R2 = Read Soldering Iron 2 Temperature (float 3.2 digits)
     *   T1,380.00 = Set Soldering Iron 1 Target Temperature (float 3.2 digits)
     *   T2,380.00 = Set Soldering Iron 2 Target Temperature (float 3.2 digits)
     *   G1,380.00 = Get Soldering Iron 1 Target Temperature (float 3.2 digits)
     *   G2,380.00 = Get Soldering Iron 2 Target Temperature (float 3.2 digits)
     *
     *   All commands end with a /n character
    */

    unsigned int bufferlength = UART1_ReceiveBufferSizeGet();
    unsigned char receivedBuffer[bufferlength];

    // check to see if the command came from the RS485 port or the Debug port
    if (uart_port == 1){
        UART1_ReadBuffer(receivedBuffer, bufferlength);
    }
    if (uart_port == 2){
        UART2_ReadBuffer(receivedBuffer, bufferlength);
    }

    #ifdef debug
       //Trace((unsigned char *)"RX");
    #endif

    // G
    if (receivedBuffer[0] == 'G'){
        

        status[0] = 'G'; status[1] = ',';
        if (SolderHead_1_GetValue()){
            status[2] = '1';
        }else{
            status[2] = '0';
        }
        status[3] = ',';
        if (SolderHead_2_GetValue()){
            status[4] = '1';
        }else{
            status[4] = '0';
        }
        status[5] = ',';
        sprintf(tempstring , "%06.2f", (double)SolderHead_1_Temperature);
        status[6] = tempstring[0];
        status[7] = tempstring[1];
        status[8] = tempstring[2];
        status[9] = tempstring[3];
        status[10] = tempstring[4];
        status[11] = tempstring[5];
        status[12] = ',';
        sprintf(tempstring , "%06.2f", (double)SolderHead_2_Temperature);
        status[13] = tempstring[0];
        status[14] = tempstring[1];
        status[15] = tempstring[2];
        status[16] = tempstring[3];
        status[17] = tempstring[4];
        status[18] = tempstring[5];
        status[19] = ',';
        sprintf(tempstring , "%06.2f", (double)SolderHead_1_Target);
        status[20] = tempstring[0];
        status[21] = tempstring[1];
        status[22] = tempstring[2];
        status[23] = tempstring[3];
        status[24] = tempstring[4];
        status[25] = tempstring[5];
        status[26] = ',';
        sprintf(tempstring , "%06.2f", (double)SolderHead_2_Target);
        status[27] = tempstring[0];
        status[28] = tempstring[1];
        status[29] = tempstring[2];
        status[30] = tempstring[3];
        status[31] = tempstring[4];
        status[32] = tempstring[5];
        status[33] = ',';
        status[34] = SolderHead_1_Activate;
        status[35] = SolderHead_2_Activate;

        RS485_WriteString((unsigned char *)status);
    }
    else{
        #ifdef debug
            Trace((unsigned char *)receivedBuffer);
        #endif

        //S1 = Read Soldering Iron 1 status (0 = off, 1 = on)
        if (receivedBuffer[0] == 'S' && receivedBuffer[1] == '1'){
            if (SolderHead_1_Activate == '1'){
                RS485_WriteString((unsigned char *)"S1,1");
                #ifdef debug
                    Trace((unsigned char *)"1");
                #endif

            }
            else{
                RS485_WriteString((unsigned char *)"S1,0");
                #ifdef debug
                    Trace((unsigned char *)"0");
                #endif
            }


        }

        //S2 = Read Soldering Iron 2 status (0 = off, 1 = on)
        if (receivedBuffer[0] == 'S' && receivedBuffer[1] == '2'){
            if (SolderHead_2_Activate == '1'){
                RS485_WriteString((unsigned char *)"S2,1");
                #ifdef debug
                    Trace((unsigned char *)"1");
                #endif
            }
            else{
                RS485_WriteString((unsigned char *)"S2,0");
                #ifdef debug
                    Trace((unsigned char *)"0");
                #endif
            }


        }

        // A,1 = Set Both Activate (0 = off, 1 = on)
        if (receivedBuffer[0] == 'A'){
            if (receivedBuffer[2] == '1'){
                SolderHead_1_Activate = '1';
                SolderHead_2_Activate = '1';

                #ifdef debug
                    Trace((unsigned char *)"Both heads on");
                #endif
            }
            else{
                SolderHead_1_Activate = '0';
                SolderHead_2_Activate = '0';
                #ifdef debug
                    Trace((unsigned char *)"Both heads off");
                #endif
            }
        }

        // I1,1 = Set Soldering Iron 1 Activate (0 = off, 1 = on)
        if (receivedBuffer[0] == 'I' && receivedBuffer[1] == '1'){
            if (receivedBuffer[3] == '1'){
                SolderHead_1_Activate = '1';
                #ifdef debug
                    Trace((unsigned char *)"Solder Head 1 on");
                #endif
            }
            else{
                SolderHead_1_Activate = '0';
                #ifdef debug
                    Trace((unsigned char *)"Solder Head 1 off");
                #endif
            }
        }

        // I2,1 = Set Soldering Iron 2 Activate (0 = off, 1 = on)
        if (receivedBuffer[0] == 'I' && receivedBuffer[1] == '2'){
            if (receivedBuffer[3] == '1'){
                SolderHead_2_Activate = '1';
                #ifdef debug
                    Trace((unsigned char *)"Solder Head 2 on");
                #endif
            }
            else{
                SolderHead_2_Activate = '0';
                #ifdef debug
                    Trace((unsigned char *)"Solder Head 2 off");
                #endif
            }
        }

        // R1 = Read Soldering Iron 1 Temperature (float 3.2 digits)
        if (receivedBuffer[0] == 'R' && receivedBuffer[1] == '1'){
            RS485_WriteString((unsigned char *)strcat("R1,",floatToChar(SolderHead_1_Temperature)));
            #ifdef debug
               Trace((unsigned char *)floatToChar(SolderHead_1_Temperature));
            #endif
        }

        // R2 = Read Soldering Iron 2 Temperature (float 3.2 digits)
        if (receivedBuffer[0] == 'R' && receivedBuffer[1] == '2'){

            RS485_WriteString((unsigned char *)strcat("R2,",floatToChar(SolderHead_2_Temperature)));
            #ifdef debug
               Trace((unsigned char *)floatToChar(SolderHead_2_Temperature));
            #endif
        }

        // T1,380.00/n = Set Soldering Iron 1 Target Temperature (float 3.2 digits)
        if (receivedBuffer[0] == 'T' && receivedBuffer[1] == '1'){
            tempstring[0] = receivedBuffer[3];
            tempstring[1] = receivedBuffer[4];
            tempstring[2] = receivedBuffer[5];
            tempstring[3] = receivedBuffer[6];
            tempstring[4] = receivedBuffer[7];
            tempstring[5] = receivedBuffer[8];

            SolderHead_1_Target = charToFloat(tempstring);

            #ifdef debug
                Trace((unsigned char *)"Solder Head 1 Temperature Set To:");
                Trace((unsigned char *)floatToChar(SolderHead_1_Target));
            #endif
        }

        // T1,380.00/n = Set Soldering Iron 1 Target Temperature (float 3.2 digits)
        if (receivedBuffer[0] == 'T' && receivedBuffer[1] == '2'){            
            tempstring[0] = receivedBuffer[3];
            tempstring[1] = receivedBuffer[4];
            tempstring[2] = receivedBuffer[5];
            tempstring[3] = receivedBuffer[6];
            tempstring[4] = receivedBuffer[7];
            tempstring[5] = receivedBuffer[8];

            SolderHead_2_Target = charToFloat(tempstring);

            #ifdef debug
                Trace((unsigned char *)"Solder Head 2 Temperature Set To:");
                Trace((unsigned char *)floatToChar(SolderHead_2_Target));
            #endif
        }

        // G1 = Read Soldering Iron 1 Target Temperature (float 3.2 digits)
        if (receivedBuffer[0] == 'G' && receivedBuffer[1] == '1'){
            RS485_WriteString((unsigned char *)strcat("G1,",floatToChar(SolderHead_1_Target)));
            #ifdef debug
               Trace((unsigned char *)floatToChar(SolderHead_1_Temperature));
            #endif
        }

        // G2 = Read Soldering Iron 2 Target Temperature (float 3.2 digits)
        if (receivedBuffer[0] == 'G' && receivedBuffer[1] == '2'){

            RS485_WriteString((unsigned char *)strcat("G2,",floatToChar(SolderHead_2_Target)));
            #ifdef debug
               Trace((unsigned char *)floatToChar(SolderHead_2_Temperature));
            #endif
        }
    }
}



/*
                         Main application
 */
int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    SDADC1_InitializeADC0();
    SDADC1_InitializeADC1();

    // get the default temperatures from flash memory
    getTemperatureDefaults();
    #ifdef debug
    Trace((unsigned char *)"Temperature Controller Starting");
    #endif

    // start timer 2 to begin checking the soldering iron temperatures
    TMR2_Start();

    #ifdef debug
    Trace((unsigned char *)"Timer started.");
    #endif

    while (1)
    {

    }

    return -1;
}

/**
 End of File
*/