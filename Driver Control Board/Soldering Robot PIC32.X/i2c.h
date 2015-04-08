/* 
 * File:   i2c.h
 * Author: andrew
 *
 * Created on 16 March 2015, 12:31
 */

void MCP4461_SetValue( unsigned char address, unsigned char channel, unsigned char value);

unsigned char MCP4461_GetValue( unsigned char address, unsigned char channel);

void MCP4461_Set_EEPROM(unsigned char address, unsigned char location, unsigned char value);

unsigned char MCP4461_Get_EEPROM(unsigned char address, unsigned char location);
