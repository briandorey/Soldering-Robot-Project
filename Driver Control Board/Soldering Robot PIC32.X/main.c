/******************************************************************************/
/*  Files to Include                                                          */
/******************************************************************************/

#ifdef __XC32
    #include <xc.h>          /* Defines special funciton registers, CP0 regs  */
#endif

#include <plib.h>           /* Include to use PIC32 peripheral libraries      */
#include <stdint.h>         /* For uint32_t definition                        */
#include <stdbool.h>        /* For true/false definition                      */


#include "system.h"         /* System funct/params, like osc/periph config    */
#include "user.h"           /* User funct/params, such as InitApp             */
#include <peripheral/outcompare.h>

#include "USB/usb.h"
#include "HardwareProfile.h"
#include "USB/usb_function_hid.h"

#include "debug.h"
#include "RS485.h"
#include "i2c.h"
#include "steppercontrol.h"

//#define debug

/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

/* i.e. uint32_t <variable_name>; */

// Define the application variables

unsigned char val_HeadStepper1_Microstep = 8;
unsigned char val_HeadStepper2_Microstep = 8;
unsigned char val_SolderStepper1_Microstep = 8;
unsigned char val_SolderStepper2_Microstep = 8;
unsigned char val_CaddyStepper_Microstep = 8;
char temperature[7];


// usb variables

#define RX_DATA_BUFFER_ADDRESS
#define TX_DATA_BUFFER_ADDRESS

unsigned char ReceivedDataBuffer[64] RX_DATA_BUFFER_ADDRESS;
unsigned char ToSendDataBuffer[64] TX_DATA_BUFFER_ADDRESS;

USB_HANDLE USBOutHandle = 0;    //USB handle.  Must be initialized to 0 at startup.
USB_HANDLE USBInHandle = 0;     //USB handle.  Must be initialized to 0 at startup.
BOOL blinkStatusValid = TRUE;


/** PRIVATE PROTOTYPES *********************************************/

static void InitializeSystem(void);
void ProcessUSBCommand(void);
void UserInit(void);
void YourHighPriorityISRCode();
void YourLowPriorityISRCode();
void USBCBSendResume(void);
WORD_VAL ReadPOT(void);
void LoadSystemDefaults(void);
void SaveSystemDefaults(void);




/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

int32_t main(void)
{
    DDPCONbits.JTAGEN = 0; // Disable the JTAG programming port

    /*
    SYS_CFG_WAIT_STATES (configures flash wait states from system clock)
    SYS_CFG_PB_BUS (configures the PB bus from the system clock)
    SYS_CFG_PCACHE (configures the pCache if used)
    SYS_CFG_ALL (configures the flash wait states, PB bus, and pCache)*/

    /* TODO Add user clock/system configuration code if appropriate.  */
    SYSTEMConfig(SYS_FREQ, SYS_CFG_ALL); 
    
    
    /*Configure Multivector Interrupt Mode.  Using Single Vector Mode
    is expensive from a timing perspective, so most applications
    should probably not use a Single Vector Mode*/
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);

    /* TODO <INSERT USER APPLICATION CODE HERE> */

    InitApp();
    initDebugPort();

    #ifdef debug
    Trace("Soldering Robot Booting...\n\r");
    #endif

    initRS485();
    initI2C();
    LoadSystemDefaults();

    USBOutHandle = 0;
    USBInHandle = 0;
    USBDeviceInit();	//usb_device.c.  Initializes USB module SFRs and firmware

    USBDeviceAttach();
    

    while(1)
    {

    }
}

void LoadSystemDefaults(void){
    // load the system defaults from eeprom
   val_HeadStepper1_Microstep = MCP4461_Get_EEPROM(0x58, 0x01);
   val_HeadStepper2_Microstep = MCP4461_Get_EEPROM(0x58, 0x02);
   val_SolderStepper1_Microstep = MCP4461_Get_EEPROM(0x58, 0x03);
   val_SolderStepper2_Microstep = MCP4461_Get_EEPROM(0x58, 0x04);
   val_CaddyStepper_Microstep = MCP4461_Get_EEPROM(0x58, 0x05);
    // set stepper driver microstepping outputs
   setHeadStepper1Microstepping(val_HeadStepper1_Microstep);
   setHeadStepper2Microstepping(val_HeadStepper2_Microstep);
   setSolderStepper1Microstepping(val_SolderStepper1_Microstep);
   setSolderStepper2Microstepping(val_SolderStepper2_Microstep);
   setCaddyStepperMicrostepping(val_CaddyStepper_Microstep);
}

// USB interrupt routine

void __ISR(_USB_1_VECTOR,IPL4AUTO) USBData(void){
    USBDeviceTasks();
    ProcessUSBCommand();
}

/********************************************************************
 * Function:        void ProcessIO(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is a place holder for other user
 *                  routines. It is a mixture of both USB and
 *                  non-USB tasks.
 *
 * Note:            None
 *******************************************************************/


void ProcessUSBCommand(void)
{
    
    // User Application USB tasks
    if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)) return;

    //Check if we have received an OUT data packet from the host
    if(!HIDRxHandleBusy(USBOutHandle))
    {
        
        //We just received a packet of data from the USB host.
        //Check the first byte of the packet to see what command the host
        //application software wants us to fulfill.
        switch (ReceivedDataBuffer[0]) {
            case 0x01: // System Commands
                switch (ReceivedDataBuffer[1]) {
                    case 0x01: // System Commands
                        #ifdef debug
                        Trace("USB Status Requested\n\r");
                        #endif

                        // Send 0xFF to show that the device is running
                        ToSendDataBuffer[0] = 0xFF;
                        // Transmit the response to the host
                        if (!HIDTxHandleBusy(USBInHandle)) {
                            USBInHandle = HIDTxPacket(HID_EP, (BYTE*) & ToSendDataBuffer[0], 64);
                        }
                        break;


                    default: // Unknown command received
                        break;
                }
                break;

            case 0x02: // Stepper Control Commands
                switch (ReceivedDataBuffer[1]) {

                    case 0x01: // Set Head Stepper 1 microstepping mode
                        val_HeadStepper1_Microstep = ReceivedDataBuffer[2];
                        setHeadStepper1Microstepping(val_HeadStepper1_Microstep);

                        MCP4461_Set_EEPROM(0x58, 0x01, val_HeadStepper1_Microstep);
                        #ifdef debug
                        Trace("USB Set Head Stepper 1 microstepping mode\n\r");
                        #endif
                        break;

                    case 0x02: // Get Head Stepper 1 microstepping mode
                        
                        ToSendDataBuffer[0] = val_HeadStepper1_Microstep;
                        // Transmit the response to the host
                        if (!HIDTxHandleBusy(USBInHandle)) {
                            USBInHandle = HIDTxPacket(HID_EP, (BYTE*) & ToSendDataBuffer[0], 64);
                        }
                        #ifdef debug
                        Trace("USB Get Head Stepper 1 microstepping mode\n\r");
                        #endif
                        break;

                    case 0x03: // Set Head Stepper 1 Current
                        MCP4461_SetValue(0x58, 0x03, ReceivedDataBuffer[2]);
                        #ifdef debug
                        Trace("USB Set Head Stepper 1 Current \n\r");
                        #endif
                        break;
                    case 0x04: // Get Head Stepper 1 Current
                        ToSendDataBuffer[0] = MCP4461_GetValue(0x58, 0x03);
                        // Transmit the response to the host
                        if (!HIDTxHandleBusy(USBInHandle)) {
                            USBInHandle = HIDTxPacket(HID_EP, (BYTE*) & ToSendDataBuffer[0], 64);
                        }
                        #ifdef debug
                        Trace("USB Get Head Stepper 1 Current\n\r");
                        #endif
                        break;
                                       
                    case 0x11: // Set Head Stepper 2 microstepping mode
                        val_HeadStepper2_Microstep = ReceivedDataBuffer[2];
                        setHeadStepper1Microstepping(val_HeadStepper2_Microstep);

                        MCP4461_Set_EEPROM(0x58, 0x02, val_HeadStepper2_Microstep);
                        #ifdef debug
                        Trace("USB Set Head Stepper 2 microstepping mode\n\r");
                        #endif
                        break;

                    case 0x12: // Get Head Stepper 2 microstepping mode

                        ToSendDataBuffer[0] = val_HeadStepper2_Microstep;
                        // Transmit the response to the host
                        if (!HIDTxHandleBusy(USBInHandle)) {
                            USBInHandle = HIDTxPacket(HID_EP, (BYTE*) & ToSendDataBuffer[0], 64);
                        }
                        #ifdef debug
                        Trace("USB Get Head Stepper 2 microstepping mode\n\r");
                        #endif
                        break;

                    case 0x13: // Set Head Stepper 2 Current
                        MCP4461_SetValue(0x58, 0x01, ReceivedDataBuffer[2]);
                        #ifdef debug
                        Trace("USB Set Head Stepper 2 Current \n\r");
                        #endif
                        break;
                    case 0x14: // Get Head Stepper 2 Current
                        ToSendDataBuffer[0] = MCP4461_GetValue(0x58, 0x01);
                        // Transmit the response to the host
                        if (!HIDTxHandleBusy(USBInHandle)) {
                            USBInHandle = HIDTxPacket(HID_EP, (BYTE*) & ToSendDataBuffer[0], 64);
                        }
                        #ifdef debug
                        Trace("USB Get Head Stepper 2 Current\n\r");
                        #endif
                        break;

                    case 0x21: // Set Solder Stepper 1 microstepping mode
                        val_SolderStepper1_Microstep = ReceivedDataBuffer[2];
                        setHeadStepper1Microstepping(val_SolderStepper1_Microstep);
                        // save the new value to the eeprom
                        MCP4461_Set_EEPROM(0x58, 0x03, val_SolderStepper1_Microstep);
                        #ifdef debug
                        Trace("USB Set Solder Stepper 2 microstepping mode\n\r");
                        #endif
                        break;

                    case 0x22: // Get Solder Stepper 1 microstepping mode

                        ToSendDataBuffer[0] = val_SolderStepper1_Microstep;
                        // Transmit the response to the host
                        if (!HIDTxHandleBusy(USBInHandle)) {
                            USBInHandle = HIDTxPacket(HID_EP, (BYTE*) & ToSendDataBuffer[0], 64);
                        }
                        #ifdef debug
                        Trace("USB Get Solder Stepper 2 microstepping mode\n\r");
                        #endif
                        break;

                    case 0x23: // Set Solder Stepper 1 Current
                        MCP4461_SetValue(0x58, 0x00, ReceivedDataBuffer[2]);
                        #ifdef debug
                        Trace("USB Set Solder Stepper 2 Current \n\r");
                        #endif
                        break;
                    case 0x24: // Get Solder Stepper 1 Current
                        ToSendDataBuffer[0] = MCP4461_GetValue(0x58, 0x00);
                        // Transmit the response to the host
                        if (!HIDTxHandleBusy(USBInHandle)) {
                            USBInHandle = HIDTxPacket(HID_EP, (BYTE*) & ToSendDataBuffer[0], 64);
                        }
                        #ifdef debug
                        Trace("USB Get Head Stepper 2 Current\n\r");
                        #endif
                        break;

                    case 0x31: // Set Solder Stepper 2 microstepping mode
                        val_SolderStepper2_Microstep = ReceivedDataBuffer[2];
                        setHeadStepper1Microstepping(val_SolderStepper2_Microstep);
                        // save the new value to the eeprom
                        MCP4461_Set_EEPROM(0x58, 0x04, val_SolderStepper2_Microstep);
                        #ifdef debug
                        Trace("USB Set Head Stepper 2 microstepping mode\n\r");
                        #endif
                        break;

                    case 0x32: // Get Solder Stepper 2 microstepping mode

                        ToSendDataBuffer[0] = val_SolderStepper2_Microstep;
                        // Transmit the response to the host
                        if (!HIDTxHandleBusy(USBInHandle)) {
                            USBInHandle = HIDTxPacket(HID_EP, (BYTE*) & ToSendDataBuffer[0], 64);
                        }
                        #ifdef debug
                        Trace("USB Get Head Stepper 2 microstepping mode\n\r");
                        #endif
                        break;

                    case 0x33: // Set Solder Stepper 2 Current
                        MCP4461_SetValue(0x5A, 0x01, ReceivedDataBuffer[2]);
                        #ifdef debug
                        Trace("USB Set Head Stepper 2 Current \n\r");
                        #endif
                        break;
                    case 0x34: // Get Solder Stepper 2 Current
                        ToSendDataBuffer[0] = MCP4461_GetValue(0x5A, 0x01);
                        // Transmit the response to the host
                        if (!HIDTxHandleBusy(USBInHandle)) {
                            USBInHandle = HIDTxPacket(HID_EP, (BYTE*) & ToSendDataBuffer[0], 64);
                        }
                        #ifdef debug
                        Trace("USB Get Head Stepper 2 Current\n\r");
                        #endif
                        break;

                    case 0x41: // Set Caddy Stepper microstepping mode
                        val_CaddyStepper_Microstep = ReceivedDataBuffer[2];
                        setHeadStepper1Microstepping(val_CaddyStepper_Microstep);
                        // save the new value to the eeprom
                        MCP4461_Set_EEPROM(0x58, 0x05, val_CaddyStepper_Microstep);
                        #ifdef debug
                        Trace("USB Set Caddy Stepper microstepping mode\n\r");
                        #endif
                        break;

                    case 0x42: // Get Caddy Stepper microstepping mode

                        ToSendDataBuffer[0] = val_CaddyStepper_Microstep;
                        // Transmit the response to the host
                        if (!HIDTxHandleBusy(USBInHandle)) {
                            USBInHandle = HIDTxPacket(HID_EP, (BYTE*) & ToSendDataBuffer[0], 64);
                        }
                        #ifdef debug
                        Trace("USB Get Caddy Stepper microstepping mode\n\r");
                        #endif
                        break;

                    case 0x43: // Set Caddy Stepper Current
                        MCP4461_SetValue(0x5A, 0x00, ReceivedDataBuffer[2]);
                        #ifdef debug
                        Trace("USB Set Caddy Stepper Current \n\r");
                        #endif
                        break;
                    case 0x44: // Get Caddy Stepper Current
                        ToSendDataBuffer[0] = MCP4461_GetValue(0x5A, 0x00);
                        // Transmit the response to the host
                        if (!HIDTxHandleBusy(USBInHandle)) {
                            USBInHandle = HIDTxPacket(HID_EP, (BYTE*) & ToSendDataBuffer[0], 64);
                        }
                        #ifdef debug
                        Trace("USB Get Caddy Stepper Current\n\r");
                        #endif
                        break;


                    default: // Unknown command received
                        break;           
                }
                break;

                case 0x03: // Soldering Iron Control Commands
                switch (ReceivedDataBuffer[1]) {

                    case 0x01: // Soldering Iron 1 Set Status
                        if(ReceivedDataBuffer[2] == 1){
                            setIron1Active(true);
                        }
                        else{
                            setIron1Active(false);
                        }
                        #ifdef debug
                        Trace("USB Soldering Iron 1 Set Status\n\r");
                        #endif
                        break;

                    case 0x02: // Soldering Iron 1 Get Running Status
                        ToSendDataBuffer[0] = SolderingIron1Status;
                        
                        // Transmit the response to the host
                        if (!HIDTxHandleBusy(USBInHandle)) {
                            USBInHandle = HIDTxPacket(HID_EP, (BYTE*) & ToSendDataBuffer[0], 64);
                        }
                        #ifdef debug
                        Trace("USB Soldering Iron 1 Get Running Status\n\r");
                        #endif
                        break;

                    case 0x03: // Soldering Iron 1 Get Temperature
                        ToSendDataBuffer[0] = SolderingIron1Temperature[0];
                        ToSendDataBuffer[1] = SolderingIron1Temperature[1];
                        ToSendDataBuffer[2] = SolderingIron1Temperature[2];
                        ToSendDataBuffer[3] = SolderingIron1Temperature[3];
                        ToSendDataBuffer[4] = SolderingIron1Temperature[4];
                        ToSendDataBuffer[5] = SolderingIron1Temperature[5];
                        // Transmit the response to the host
                        if (!HIDTxHandleBusy(USBInHandle)) {
                            USBInHandle = HIDTxPacket(HID_EP, (BYTE*) & ToSendDataBuffer[0], 64);
                        }
                        #ifdef debug
                        Trace("USB Soldering Iron 1 Get Temperature\n\r");
                        #endif
                        break;
                    case 0x04: // Soldering Iron 1 Get Target Temperature
                        ToSendDataBuffer[0] = SolderingIron1TargetTemperature[0];
                        ToSendDataBuffer[1] = SolderingIron1TargetTemperature[1];
                        ToSendDataBuffer[2] = SolderingIron1TargetTemperature[2];
                        ToSendDataBuffer[3] = SolderingIron1TargetTemperature[3];
                        ToSendDataBuffer[4] = SolderingIron1TargetTemperature[4];
                        ToSendDataBuffer[5] = SolderingIron1TargetTemperature[5];
                        // Transmit the response to the host
                        if (!HIDTxHandleBusy(USBInHandle)) {
                            USBInHandle = HIDTxPacket(HID_EP, (BYTE*) & ToSendDataBuffer[0], 64);
                        }
                        #ifdef debug
                        Trace("USB Soldering Iron 1 Get Target Temperature\n\r");
                        #endif
                        break;
                    case 0x05: // Soldering Iron 1 Set Target Temperature
                        
                        temperature[0] = ReceivedDataBuffer[2];
                        temperature[1] = ReceivedDataBuffer[3];
                        temperature[2] = ReceivedDataBuffer[4];
                        temperature[3] = ReceivedDataBuffer[5];
                        temperature[4] = ReceivedDataBuffer[6];
                        temperature[5] = ReceivedDataBuffer[7];

                        setIron1Temperature(temperature);
                        #ifdef debug
                        Trace("USB Soldering Iron 1 Set Target Temperature\n\r");
                        #endif
                        
                        break;

                    case 0x06: // Soldering Iron 1 Get Activation Status
                        ToSendDataBuffer[0] = SolderingIron1Activate;

                        // Transmit the response to the host
                        if (!HIDTxHandleBusy(USBInHandle)) {
                            USBInHandle = HIDTxPacket(HID_EP, (BYTE*) & ToSendDataBuffer[0], 64);
                        }
                        #ifdef debug
                        Trace("USB Soldering Iron 1 Get Activation Status\n\r");
                        #endif
                        break;

                    case 0x11: // Soldering Iron 2 Set Status
                        if(ReceivedDataBuffer[2] == 1){
                            setIron2Active(true);
                        }
                        else{
                            setIron2Active(false);
                        }
                        #ifdef debug
                        Trace("USB Soldering Iron 2 Set Status\n\r");
                        #endif
                        break;

                    case 0x12: // Soldering Iron 2 Get Active Status
                        ToSendDataBuffer[0] = SolderingIron2Status;

                        // Transmit the response to the host
                        if (!HIDTxHandleBusy(USBInHandle)) {
                            USBInHandle = HIDTxPacket(HID_EP, (BYTE*) & ToSendDataBuffer[0], 64);
                        }
                        #ifdef debug
                        Trace("USB Soldering Iron 2 Get Active Status\n\r");
                        #endif
                        break;
                    case 0x13: // Soldering Iron 2 Get Temperature
                        ToSendDataBuffer[0] = SolderingIron2Temperature[0];
                        ToSendDataBuffer[1] = SolderingIron2Temperature[1];
                        ToSendDataBuffer[2] = SolderingIron2Temperature[2];
                        ToSendDataBuffer[3] = SolderingIron2Temperature[3];
                        ToSendDataBuffer[4] = SolderingIron2Temperature[4];
                        ToSendDataBuffer[5] = SolderingIron2Temperature[5];
                        // Transmit the response to the host
                        if (!HIDTxHandleBusy(USBInHandle)) {
                            USBInHandle = HIDTxPacket(HID_EP, (BYTE*) & ToSendDataBuffer[0], 64);
                        }
                        #ifdef debug
                        Trace("USB Soldering Iron 2 Get Temperature\n\r");
                        #endif
                        break;
                    case 0x14: // Soldering Iron 2 Get Target Temperature
                        ToSendDataBuffer[0] = SolderingIron2TargetTemperature[0];
                        ToSendDataBuffer[1] = SolderingIron2TargetTemperature[1];
                        ToSendDataBuffer[2] = SolderingIron2TargetTemperature[2];
                        ToSendDataBuffer[3] = SolderingIron2TargetTemperature[3];
                        ToSendDataBuffer[4] = SolderingIron2TargetTemperature[4];
                        ToSendDataBuffer[5] = SolderingIron2TargetTemperature[5];
                        // Transmit the response to the host
                        if (!HIDTxHandleBusy(USBInHandle)) {
                            USBInHandle = HIDTxPacket(HID_EP, (BYTE*) & ToSendDataBuffer[0], 64);
                        }
                        #ifdef debug
                        Trace("USB Soldering Iron 2 Get Target Temperature\n\r");
                        #endif
                        break;
                    case 0x15: // Soldering Iron 2 Set Target Temperature

                        temperature[0] = ReceivedDataBuffer[2];
                        temperature[1] = ReceivedDataBuffer[3];
                        temperature[2] = ReceivedDataBuffer[4];
                        temperature[3] = ReceivedDataBuffer[5];
                        temperature[4] = ReceivedDataBuffer[6];
                        temperature[5] = ReceivedDataBuffer[7];
                        setIron2Temperature(temperature);
                        #ifdef debug
                        Trace("USB Soldering Iron 2 Set Target Temperature\n\r");
                        #endif
                        break;

                    case 0x16: // Soldering Iron 2 Get Activation Status
                        ToSendDataBuffer[0] = SolderingIron2Activate;

                        // Transmit the response to the host
                        if (!HIDTxHandleBusy(USBInHandle)) {
                            USBInHandle = HIDTxPacket(HID_EP, (BYTE*) & ToSendDataBuffer[0], 64);
                        }
                        #ifdef debug
                        Trace("USB Soldering Iron 2 Get Activation Status\n\r");
                        #endif
                        break;
                }
                break;



            default: // Unknown command received
                break;
        }
        //Re-arm the OUT endpoint, so we can receive the next OUT data packet
        //that the host may try to send us.
        USBOutHandle = HIDRxPacket(HID_EP, (BYTE*)&ReceivedDataBuffer, 64);
    }


}//end ProcessIO



// ******************************************************************************************************
// ************** USB Callback Functions ****************************************************************
// ******************************************************************************************************
// The USB firmware stack will call the callback functions USBCBxxx() in response to certain USB related
// events.  For example, if the host PC is powering down, it will stop sending out Start of Frame (SOF)
// packets to your device.  In response to this, all USB devices are supposed to decrease their power
// consumption from the USB Vbus to <2.5mA* each.  The USB module detects this condition (which according
// to the USB specifications is 3+ms of no bus activity/SOF packets) and then calls the USBCBSuspend()
// function.  You should modify these callback functions to take appropriate actions for each of these
// conditions.  For example, in the USBCBSuspend(), you may wish to add code that will decrease power
// consumption from Vbus to <2.5mA (such as by clock switching, turning off LEDs, putting the
// microcontroller to sleep, etc.).  Then, in the USBCBWakeFromSuspend() function, you may then wish to
// add code that undoes the power saving things done in the USBCBSuspend() function.

// The USBCBSendResume() function is special, in that the USB stack will not automatically call this
// function.  This function is meant to be called from the application firmware instead.  See the
// additional comments near the function.

// Note *: The "usb_20.pdf" specs indicate 500uA or 2.5mA, depending upon device classification. However,
// the USB-IF has officially issued an ECN (engineering change notice) changing this to 2.5mA for all
// devices.  Make sure to re-download the latest specifications to get all of the newest ECNs.

/******************************************************************************
 * Function:        void USBCBSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Call back that is invoked when a USB suspend is detected
 *
 * Note:            None
 *****************************************************************************/
void USBCBSuspend(void)
{
	//Example power saving code.  Insert appropriate code here for the desired
	//application behavior.  If the microcontroller will be put to sleep, a
	//process similar to that shown below may be used:

	//ConfigureIOPinsForLowPower();
	//SaveStateOfAllInterruptEnableBits();
	//DisableAllInterruptEnableBits();
	//EnableOnlyTheInterruptsWhichWillBeUsedToWakeTheMicro();	//should enable at least USBActivityIF as a wake source
	//Sleep();
	//RestoreStateOfAllPreviouslySavedInterruptEnableBits();	//Preferrably, this should be done in the USBCBWakeFromSuspend() function instead.
	//RestoreIOPinsToNormal();									//Preferrably, this should be done in the USBCBWakeFromSuspend() function instead.

	//IMPORTANT NOTE: Do not clear the USBActivityIF (ACTVIF) bit here.  This bit is
	//cleared inside the usb_device.c file.  Clearing USBActivityIF here will cause
	//things to not work as intended.


    #if defined(__C30__) || defined __XC16__
        //This function requires that the _IPL level be something other than 0.
        //  We can set it here to something other than
        #ifndef DSPIC33E_USB_STARTER_KIT
        _IPL = 1;
        USBSleepOnSuspend();
        #endif
    #endif
}



/******************************************************************************
 * Function:        void USBCBWakeFromSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The host may put USB peripheral devices in low power
 *					suspend mode (by "sending" 3+ms of idle).  Once in suspend
 *					mode, the host may wake the device back up by sending non-
 *					idle state signalling.
 *
 *					This call back is invoked when a wakeup from USB suspend
 *					is detected.
 *
 * Note:            None
 *****************************************************************************/
void USBCBWakeFromSuspend(void)
{
	// If clock switching or other power savings measures were taken when
	// executing the USBCBSuspend() function, now would be a good time to
	// switch back to normal full power run mode conditions.  The host allows
	// 10+ milliseconds of wakeup time, after which the device must be
	// fully back to normal, and capable of receiving and processing USB
	// packets.  In order to do this, the USB module must receive proper
	// clocking (IE: 48MHz clock must be available to SIE for full speed USB
	// operation).
	// Make sure the selected oscillator settings are consistent with USB
    // operation before returning from this function.
}

/********************************************************************
 * Function:        void USBCB_SOF_Handler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USB host sends out a SOF packet to full-speed
 *                  devices every 1 ms. This interrupt may be useful
 *                  for isochronous pipes. End designers should
 *                  implement callback routine as necessary.
 *
 * Note:            None
 *******************************************************************/
void USBCB_SOF_Handler(void)
{
    // No need to clear UIRbits.SOFIF to 0 here.
    // Callback caller is already doing that.
}

/*******************************************************************
 * Function:        void USBCBErrorHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The purpose of this callback is mainly for
 *                  debugging during development. Check UEIR to see
 *                  which error causes the interrupt.
 *
 * Note:            None
 *******************************************************************/
void USBCBErrorHandler(void)
{
    // No need to clear UEIR to 0 here.
    // Callback caller is already doing that.

	// Typically, user firmware does not need to do anything special
	// if a USB error occurs.  For example, if the host sends an OUT
	// packet to your device, but the packet gets corrupted (ex:
	// because of a bad connection, or the user unplugs the
	// USB cable during the transmission) this will typically set
	// one or more USB error interrupt flags.  Nothing specific
	// needs to be done however, since the SIE will automatically
	// send a "NAK" packet to the host.  In response to this, the
	// host will normally retry to send the packet again, and no
	// data loss occurs.  The system will typically recover
	// automatically, without the need for application firmware
	// intervention.

	// Nevertheless, this callback function is provided, such as
	// for debugging purposes.
}


/*******************************************************************
 * Function:        void USBCBCheckOtherReq(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        When SETUP packets arrive from the host, some
 * 					firmware must process the request and respond
 *					appropriately to fulfill the request.  Some of
 *					the SETUP packets will be for standard
 *					USB "chapter 9" (as in, fulfilling chapter 9 of
 *					the official USB specifications) requests, while
 *					others may be specific to the USB device class
 *					that is being implemented.  For example, a HID
 *					class device needs to be able to respond to
 *					"GET REPORT" type of requests.  This
 *					is not a standard USB chapter 9 request, and
 *					therefore not handled by usb_device.c.  Instead
 *					this request should be handled by class specific
 *					firmware, such as that contained in usb_function_hid.c.
 *
 * Note:            None
 *******************************************************************/
void USBCBCheckOtherReq(void)
{
    USBCheckHIDRequest();
}//end


/*******************************************************************
 * Function:        void USBCBStdSetDscHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USBCBStdSetDscHandler() callback function is
 *					called when a SETUP, bRequest: SET_DESCRIPTOR request
 *					arrives.  Typically SET_DESCRIPTOR requests are
 *					not used in most applications, and it is
 *					optional to support this type of request.
 *
 * Note:            None
 *******************************************************************/
void USBCBStdSetDscHandler(void)
{
    // Must claim session ownership if supporting this request
}//end


/*******************************************************************
 * Function:        void USBCBInitEP(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called when the device becomes
 *                  initialized, which occurs after the host sends a
 * 					SET_CONFIGURATION (wValue not = 0) request.  This
 *					callback function should initialize the endpoints
 *					for the device's usage according to the current
 *					configuration.
 *
 * Note:            None
 *******************************************************************/
void USBCBInitEP(void)
{
    //enable the HID endpoint
    USBEnableEndpoint(HID_EP,USB_IN_ENABLED|USB_OUT_ENABLED|USB_HANDSHAKE_ENABLED|USB_DISALLOW_SETUP);
    //Re-arm the OUT endpoint for the next packet
    USBOutHandle = HIDRxPacket(HID_EP,(BYTE*)&ReceivedDataBuffer,64);
}

/********************************************************************
 * Function:        void USBCBSendResume(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USB specifications allow some types of USB
 * 					peripheral devices to wake up a host PC (such
 *					as if it is in a low power suspend to RAM state).
 *					This can be a very useful feature in some
 *					USB applications, such as an Infrared remote
 *					control	receiver.  If a user presses the "power"
 *					button on a remote control, it is nice that the
 *					IR receiver can detect this signalling, and then
 *					send a USB "command" to the PC to wake up.
 *
 *					The USBCBSendResume() "callback" function is used
 *					to send this special USB signalling which wakes
 *					up the PC.  This function may be called by
 *					application firmware to wake up the PC.  This
 *					function will only be able to wake up the host if
 *                  all of the below are true:
 *
 *					1.  The USB driver used on the host PC supports
 *						the remote wakeup capability.
 *					2.  The USB configuration descriptor indicates
 *						the device is remote wakeup capable in the
 *						bmAttributes field.
 *					3.  The USB host PC is currently sleeping,
 *						and has previously sent your device a SET
 *						FEATURE setup packet which "armed" the
 *						remote wakeup capability.
 *
 *                  If the host has not armed the device to perform remote wakeup,
 *                  then this function will return without actually performing a
 *                  remote wakeup sequence.  This is the required behavior,
 *                  as a USB device that has not been armed to perform remote
 *                  wakeup must not drive remote wakeup signalling onto the bus;
 *                  doing so will cause USB compliance testing failure.
 *
 *					This callback should send a RESUME signal that
 *                  has the period of 1-15ms.
 *
 * Note:            This function does nothing and returns quickly, if the USB
 *                  bus and host are not in a suspended condition, or are
 *                  otherwise not in a remote wakeup ready state.  Therefore, it
 *                  is safe to optionally call this function regularly, ex:
 *                  anytime application stimulus occurs, as the function will
 *                  have no effect, until the bus really is in a state ready
 *                  to accept remote wakeup.
 *
 *                  When this function executes, it may perform clock switching,
 *                  depending upon the application specific code in
 *                  USBCBWakeFromSuspend().  This is needed, since the USB
 *                  bus will no longer be suspended by the time this function
 *                  returns.  Therefore, the USB module will need to be ready
 *                  to receive traffic from the host.
 *
 *                  The modifiable section in this routine may be changed
 *                  to meet the application needs. Current implementation
 *                  temporary blocks other functions from executing for a
 *                  period of ~3-15 ms depending on the core frequency.
 *
 *                  According to USB 2.0 specification section 7.1.7.7,
 *                  "The remote wakeup device must hold the resume signaling
 *                  for at least 1 ms but for no more than 15 ms."
 *                  The idea here is to use a delay counter loop, using a
 *                  common value that would work over a wide range of core
 *                  frequencies.
 *                  That value selected is 1800. See table below:
 *                  ==========================================================
 *                  Core Freq(MHz)      MIP         RESUME Signal Period (ms)
 *                  ==========================================================
 *                      48              12          1.05
 *                       4              1           12.6
 *                  ==========================================================
 *                  * These timing could be incorrect when using code
 *                    optimization or extended instruction mode,
 *                    or when having other interrupts enabled.
 *                    Make sure to verify using the MPLAB SIM's Stopwatch
 *                    and verify the actual signal on an oscilloscope.
 *******************************************************************/
void USBCBSendResume(void)
{
    static WORD delay_count;

    //First verify that the host has armed us to perform remote wakeup.
    //It does this by sending a SET_FEATURE request to enable remote wakeup,
    //usually just before the host goes to standby mode (note: it will only
    //send this SET_FEATURE request if the configuration descriptor declares
    //the device as remote wakeup capable, AND, if the feature is enabled
    //on the host (ex: on Windows based hosts, in the device manager
    //properties page for the USB device, power management tab, the
    //"Allow this device to bring the computer out of standby." checkbox
    //should be checked).
    if(USBGetRemoteWakeupStatus() == TRUE)
    {
        //Verify that the USB bus is in fact suspended, before we send
        //remote wakeup signalling.
        if(USBIsBusSuspended() == TRUE)
        {
            USBMaskInterrupts();

            //Clock switch to settings consistent with normal USB operation.
            USBCBWakeFromSuspend();
            USBSuspendControl = 0;
            USBBusIsSuspended = FALSE;  //So we don't execute this code again,
                                        //until a new suspend condition is detected.

            //Section 7.1.7.7 of the USB 2.0 specifications indicates a USB
            //device must continuously see 5ms+ of idle on the bus, before it sends
            //remote wakeup signalling.  One way to be certain that this parameter
            //gets met, is to add a 2ms+ blocking delay here (2ms plus at
            //least 3ms from bus idle to USBIsBusSuspended() == TRUE, yeilds
            //5ms+ total delay since start of idle).
            delay_count = 3600U;
            do
            {
                delay_count--;
            }while(delay_count);

            //Now drive the resume K-state signalling onto the USB bus.
            USBResumeControl = 1;       // Start RESUME signaling
            delay_count = 1800U;        // Set RESUME line for 1-13 ms
            do
            {
                delay_count--;
            }while(delay_count);
            USBResumeControl = 0;       //Finished driving resume signalling

            USBUnmaskInterrupts();
        }
    }
}


/*******************************************************************
 * Function:        BOOL USER_USB_CALLBACK_EVENT_HANDLER(
 *                        USB_EVENT event, void *pdata, WORD size)
 *
 * PreCondition:    None
 *
 * Input:           USB_EVENT event - the type of event
 *                  void *pdata - pointer to the event data
 *                  WORD size - size of the event data
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called from the USB stack to
 *                  notify a user application that a USB event
 *                  occured.  This callback is in interrupt context
 *                  when the USB_INTERRUPT option is selected.
 *
 * Note:            None
 *******************************************************************/
BOOL USER_USB_CALLBACK_EVENT_HANDLER(int event, void *pdata, WORD size)
{
    switch(event)
    {
        case EVENT_TRANSFER:
            //Add application specific callback task or callback function here if desired.
            break;
        case EVENT_SOF:
            USBCB_SOF_Handler();
            break;
        case EVENT_SUSPEND:
            USBCBSuspend();
            break;
        case EVENT_RESUME:
            USBCBWakeFromSuspend();
            break;
        case EVENT_CONFIGURED:
            USBCBInitEP();
            break;
        case EVENT_SET_DESCRIPTOR:
            USBCBStdSetDscHandler();
            break;
        case EVENT_EP0_REQUEST:
            USBCBCheckOtherReq();
            break;
        case EVENT_BUS_ERROR:
            USBCBErrorHandler();
            break;
        case EVENT_TRANSFER_TERMINATED:
            //Add application specific callback task or callback function here if desired.
            //The EVENT_TRANSFER_TERMINATED event occurs when the host performs a CLEAR
            //FEATURE (endpoint halt) request on an application endpoint which was
            //previously armed (UOWN was = 1).  Here would be a good place to:
            //1.  Determine which endpoint the transaction that just got terminated was
            //      on, by checking the handle value in the *pdata.
            //2.  Re-arm the endpoint if desired (typically would be the case for OUT
            //      endpoints).
            break;
        default:
            break;
    }
    return TRUE;
}

/** EOF main.c *************************************************/

