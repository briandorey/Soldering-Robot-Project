/*
 *
 * I2C functions
 *
 * This library deals with I2C communictaion to the MCP4461 digital potentiometers
 *
 * Uses I2C 2
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

#include "HardwareProfile.h"
#include "debug.h"

#define I2C_CLOCK_FREQ             1000000

// Initialise the I2C bus

void initI2C(void) {
    // Enable the I2C bus
    OpenI2C2(I2C_EN, I2C_CLOCK_FREQ);
}


// Write a value to one of the digital pots

void MCP4461_SetValue(unsigned char address, unsigned char channel, unsigned char value) {

    char nonvolatile_address = 0;
    char volatile_address = 0;
    switch (channel) {
        case 0:
            nonvolatile_address = 0x20;
            volatile_address = 0x00;
            break;
        case 1:
            nonvolatile_address = 0x30;
            volatile_address = 0x10;
            break;
        case 2:
            nonvolatile_address = 0x80;
            volatile_address = 0x60;
            break;
        case 3:
            nonvolatile_address = 0x90;
            volatile_address = 0x70;
            break;
    }

    StartI2C2(); //Send the Start Bit
    IdleI2C2(); //Wait to complete

    

    // Send the new value to the volatile memory space, updates the pot output voltage

    MasterWriteI2C2(address); //Sends the slave address over the I2C line.
    IdleI2C2(); //Wait to complete
    MasterWriteI2C2(volatile_address); //Sends data byte over I2C line
    IdleI2C2(); //Wait to complete
    MasterWriteI2C2(value); //Sends data byte over I2C line
    IdleI2C2(); //Wait to complete
    StopI2C2(); //Send the Stop condition
    IdleI2C2(); //Wait to complete

    // Send the new value to the nonvolatile memory space, needed to keep the pot at the correct voltage on startup
    StartI2C2(); //Send the Start Bit
    IdleI2C2(); //Wait to complete
    MasterWriteI2C2(address); //Sends the slave address over the I2C line.
    IdleI2C2(); //Wait to complete
    MasterWriteI2C2(nonvolatile_address); //Sends data byte over I2C line
    IdleI2C2(); //Wait to complete
    MasterWriteI2C2(value); //Sends data byte over I2C line
    IdleI2C2(); //Wait to complete
    StopI2C2(); //Send the Stop condition
    IdleI2C2(); //Wait to complete

}

// Read a value from one of the digital pots

unsigned char MCP4461_GetValue(unsigned char address, unsigned char channel) {

    unsigned char string[3];
    unsigned char *rdptr;
    unsigned int length, i2c_data_wait;
    length = 2;
    rdptr = string;
    i2c_data_wait = 10000;

    char volatile_address = 0;
    // get the memory address based on the selected channel
    switch (channel) {
        case 0:
            volatile_address = 0x0C;
            break;
        case 1:
            volatile_address = 0x1C;
            break;
        case 2:
            volatile_address = 0x6C;
            break;
        case 3:
            volatile_address = 0x7C;
            break;
    }

    // write the memory address to the device
    StartI2C2();			
    IdleI2C2();			       
    MasterWriteI2C2(address); 
    IdleI2C2(); 
    MasterWriteI2C2(volatile_address); 
    IdleI2C2(); 
    StopI2C2();				
    IdleI2C2();

    // read the pot value from the device
    StartI2C2();				
    IdleI2C2();			        
    MasterWriteI2C2(address + 1);
    IdleI2C2();				
    MastergetsI2C2(length, rdptr, i2c_data_wait);		
    StopI2C2();				
    IdleI2C2();				

    return rdptr[1];				//Return read value
}


// Write a value to the EEPROM

void MCP4461_Set_EEPROM(unsigned char address, unsigned char location, unsigned char value){

    unsigned char eeprom_address;

    switch (location){
        case 1:
            eeprom_address = 0xb0;
            break;
        case 2:
            eeprom_address = 0xc0;
            break;
        case 3:
            eeprom_address = 0xd0;
            break;
        case 4:
            eeprom_address = 0xe0;
            break;
        case 5:
            eeprom_address = 0xf0;
            break;
    }


      // Send the new value to the nonvolatile memory space, needed to keep the pot at the correct voltage on startup
    StartI2C2(); //Send the Start Bit
    IdleI2C2(); //Wait to complete
    MasterWriteI2C2(address); //Sends the slave address over the I2C line.
    IdleI2C2(); //Wait to complete
    MasterWriteI2C2(eeprom_address); //Sends data byte over I2C line
    IdleI2C2(); //Wait to complete
    MasterWriteI2C2(value); //Sends data byte over I2C line
    IdleI2C2(); //Wait to complete
    StopI2C2(); //Send the Stop condition
    IdleI2C2(); //Wait to complete

}

// Read a value from the EEPROM

unsigned char MCP4461_Get_EEPROM(unsigned char address, unsigned char location){
    unsigned char eeprom_address;
    unsigned char *rdptr;
    unsigned int length, i2c_data_wait;
    unsigned char string[3];
    length = 2;
    rdptr = string;
    i2c_data_wait = 10000;

    switch (location){
        case 1:
            eeprom_address = 0xbc;
            break;
        case 2:
            eeprom_address = 0xcc;
            break;
        case 3:
            eeprom_address = 0xdc;
            break;
        case 4:
            eeprom_address = 0xec;
            break;
        case 5:
            eeprom_address = 0xfc;
            break;
    }

        StartI2C2();
        IdleI2C2();
        MasterWriteI2C2(address);
        IdleI2C2();
        MasterWriteI2C2(eeprom_address);
        IdleI2C2();
        StopI2C2();
        IdleI2C2();

       StartI2C2();
        IdleI2C2();
        MasterWriteI2C2(address + 1);
        IdleI2C2();
        MastergetsI2C2(length, rdptr, i2c_data_wait);
        StopI2C2();
        IdleI2C2();

    return rdptr[1];				//Return read value
}