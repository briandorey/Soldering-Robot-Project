/*******************************************************************************
  SDADC1 Generated Driver API Header File

  Company:
    Microchip Technology Inc.

  File Name:
    sdadc1.h

  Summary:
    This is the generated header file for the SDADC1 driver using MPLAB® Code Configurator

  Description:
    This header file provides APIs for driver for SDADC1.
    Generation Information :
        Product Revision  :  MPLAB® Code Configurator - v2.10.2
        Device            :  PIC24FJ128GC006
        Version           :  1.0
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.24
        MPLAB             :  MPLAB X 2.26
*******************************************************************************/

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

#ifndef SDADC1_H
#define SDADC1_H
/**
  Section: Included Files
*/
#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
/**
  Section: Data Types
*/

/** SD ADC Channel Definition
 
 @Summary 
   Defines the channels available for conversion
 
 @Description
   This data type defines the channels that are available for the module to use.
 
 Remarks:
   None
 */
typedef enum
{
    SDADC1_CH_REFERENCE = 0x3,
    SDADC1_CH1_SINGLE = 0x2,
    SDADC1_CH1_DIFFERENTIAL = 0x1,
    SDADC1_CH0_DIFFERENTIAL = 0x0
}SDADC1_CHANNEL;

/**
  Section: Interface Routines
*/
/**
  @Summary
    Initializes hardware and data for the given instance of the Sigma Delta ADC Module.

  @Description
    This routine initializes hardware for the instance of the SD ADC Module,
    using the hardware initialization data given in the UI.  It also performs Calibration of the module.
   
  @Param
    None.

  @Returns
    None
 
  @Example 
    <code>
    float fResult;
    bool bConversionComplete;
	
    SDADC1_InitializeADC0();
    while (1)
    {
		bConversionComplete = SDADC1_IsConversionComplete();
        if(true == bConversionComplete)
        {
            fResult = SDADC1_ConversionResultGet();
        }
    }
    </code>
*/
void SDADC1_InitializeADC0(void);
/**
  @Summary
    Initializes hardware and data for the given instance of the Sigma Delta ADC Module.

  @Description
    This routine initializes hardware for the instance of the SD ADC Module,
    using the hardware initialization data given in the UI.  It also performs Calibration of the module.
   
  @Param
    None.

  @Returns
    None
 
  @Example 
    <code>
    float fResult;
    bool bConversionComplete;
	
    SDADC1_InitializeADC1();
    while (1)
    {
		bConversionComplete = SDADC1_IsConversionComplete();
        if(true == bConversionComplete)
        {
            fResult = SDADC1_ConversionResultGet();
        }
    }
    </code>
*/
void SDADC1_InitializeADC1(void);
/**
  @Summary
    Selects the SD ADC channel for conversion.

  @Description
    This routine selects the specified channel for Conversion, by
    using the channel enumeration passed as a parameter.
   
  @Param
    eChannel: Selects channel for conversion, see SDADC1_CHANNEL.
    for options available for channel selection. 

  @Returns
    None
 
  @Example 
    <code>
    float fResult;
    bool bConversionComplete;
	
    SDADC1_ChannelSelect(SDADC1_CH1_SINGLE);
	bConversionComplete = SDADC1_IsConversionComplete();
	if(true == bConversionComplete)
	{
		fResult = SDADC1_ConversionResultGet();
	}
    </code>
*/
void SDADC1_ChannelSelect(SDADC1_CHANNEL eChannel);
/**
  @Summary
    Returns the status of conversion.

  @Description
    This routine reads the register flags and returns the status of converion.
   
  @Param
    None

  @Returns
    true - Conversion complete and new data is available for reading.
	false - Conversion not yet complete.
 
  @Example 
    refer to the example of initializer routine above.
*/
bool SDADC1_IsConversionComplete(void);
/**
  @Summary
    Returns the raw result for the channel selected earlier.

  @Description
    This routine returns the raw value after conversion by SD ADC.
	If the conversion is complete, it immediately returns the raw result.
	If the conversion is not complete, it waits till the conversion is completed
	and then returns the raw result.
   
  @Param
    None

  @Returns
    Signed 16-bit result.
 
  @Example 
    <code>
    int16_t i16Result;
    bool bConversionComplete;

	bConversionComplete = SDADC1_IsConversionComplete();
	if(true == bConversionComplete)
	{
		i16Result = SDADC1_ConversionRawResultGet();
	}
    </code>
*/
int16_t SDADC1_ConversionRawResultGet(void);
/**
  @Summary
    Returns the calibrated result for the channel selected earlier.

  @Description
    This routine returns the calibrated result value after conversion by SD ADC.
	If the conversion is complete, it immediately returns the calibrated result.
	If the conversion is not complete, it waits till the conversion is completed
	and then returns the calibrated result.
	The calibration values used here are computed during the initialization of the SD ADC.
   
  @Param
    None

  @Returns
    floating point calibrated result.
 
  @Example 
    refer to the example of initializer routine above.
*/
float SDADC1_ConversionResultGet(void);

void __attribute__ (( interrupt, no_auto_psv )) _ISR _SDA1Interrupt( void );
#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif
#endif  // SDADC1.h