/* 
 * File:   HardwareProfile.h
 * Author: andrew
 *
 * Created on 10 June 2014, 16:40
 */

#ifndef HARDWAREPROFILE_H
#define	HARDWAREPROFILE_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* HARDWAREPROFILE_H */

// PIC to hardware pin mapping and control macros

#define HeadStepper1_Enable LATBbits.LATB3
#define HeadStepper1_Step LATBbits.LATB4
#define HeadStepper1_Direction LATBbits.LATB2
#define HeadStepper1_Mode0 LATEbits.LATE9
#define HeadStepper1_Mode1 LATEbits.LATE8
#define HeadStepper1_Mode2 LATAbits.LATA0

#define HeadStepper2_Enable LATBbits.LATB8
#define HeadStepper2_Step LATAbits.LATA10
#define HeadStepper2_Direction LATBbits.LATB9
#define HeadStepper2_Mode0 LATAbits.LATA9
#define HeadStepper2_Mode1 LATBbits.LATB7
#define HeadStepper2_Mode2 LATBbits.LATB6

#define SolderStepper1_Enable LATFbits.LATF12
#define SolderStepper1_Step LATFbits.LATF13
#define SolderStepper1_Direction LATBbits.LATB12
#define SolderStepper1_Mode0 LATAbits.LATA1
#define SolderStepper1_Mode1 LATBbits.LATB11
#define SolderStepper1_Mode2 LATBbits.LATB10

#define SolderStepper2_Enable LATFbits.LATF5
#define SolderStepper2_Step LATFbits.LATF4
#define SolderStepper2_Direction LATBbits.LATB15
#define SolderStepper2_Mode0 LATDbits.LATD15
#define SolderStepper2_Mode1 LATDbits.LATD14
#define SolderStepper2_Mode2 LATBbits.LATB14

#define CaddyStepper_Enable LATDbits.LATD0
#define CaddyStepper_Step LATDbits.LATD11
#define CaddyStepper_Direction LATCbits.LATC13
#define CaddyStepper_Mode0 LATDbits.LATD10
#define CaddyStepper_Mode1 LATDbits.LATD9
#define CaddyStepper_Mode2 LATDbits.LATD8

#define Endoder_A LATCbits.LATC1
#define Encoder_B LATCbits.LATC2

#define Sensor_Head1 LATDbits.LATD1
#define Sensor_Head2 LATDbits.LATD2
#define Sensor_SolderFeed1 LATDbits.LATD3
#define Sensor_SolderFeed2 LATDbits.LATD12
#define Sensor_Caddy LATDbits.LATD13


// Bluetooth Definitions

#define Bluetooth_RX_Pin LATFbits.LATF2
#define Bluetooth_TX_Pin LATFbits.LATF8
#define Bluetooth_Command_Pin LATCbits.LATC14

// Debug Definitions

#define Debug_RX_Pin LATGbits.LATG9
#define Debug_TX_Pin LATGbits.LATG6

// RS485 Definitions

#define RS485_RX_Pin LATGbits.LATG7
#define RS485_TX_Pin LATGbits.LATG8

// I2C Definitions

#define SDA_Pin LATAbits.LATA3
#define SCL_Pin LATAbits.LATA2



 /*******************************************************************/
    /******** USB stack hardware selection options *********************/
    /*******************************************************************/
    //This section is the set of definitions required by the MCHPFSUSB
    //  framework.  These definitions tell the firmware what mode it is
    //  running in, and where it can find the results to some information
    //  that the stack needs.
    //These definitions are required by every application developed with
    //  this revision of the MCHPFSUSB framework.  Please review each
    //  option carefully and determine which options are desired/required
    //  for your application.

    //#define USE_SELF_POWER_SENSE_IO
    #define tris_self_power     TRISAbits.TRISA2    // Input
    #define self_power          1

    //#define USE_USB_BUS_SENSE_IO
    #define tris_usb_bus_sense  TRISBbits.TRISB5    // Input
    #define USB_BUS_SENSE       1

// Device Vendor Indentifier (VID) (0x04D8 is Microchip's VID)
#define USB_VID	0x04D8

// Device Product Indentifier (PID) (0x0042)
#define USB_PID	0x0045

// Manufacturer string descriptor
#define MSDLENGTH	17
#define MSD		'A','B',' ','E','l','e','c','t','r','o','n','i','c','s',' ','U','K'

// Product String descriptor
#define PSDLENGTH	15
#define PSD		'S','o','l','d','e','r','i','n','g',' ','R','o','b','o','t'

// Device serial number string descriptor
#define DSNLENGTH	7
#define DSN		'V','e','r','_','1','.','0'