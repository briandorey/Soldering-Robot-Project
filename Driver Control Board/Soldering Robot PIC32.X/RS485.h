/* 
 * File:   RS485.h
 * Author: andrew
 *
 * Created on 16 March 2015, 12:31
 */

// soldering iron variables

extern unsigned char SolderingIron1Status;
extern unsigned char SolderingIron2Status;
extern unsigned char SolderingIron1Activate;
extern unsigned char SolderingIron2Activate;
extern unsigned char SolderingIron1Temperature[6];
extern unsigned char SolderingIron2Temperature[6];
extern unsigned char SolderingIron1TargetTemperature[6];
extern unsigned char SolderingIron2TargetTemperature[6];

void initRS485(void);

void setBothIronsActive(bool status);

void setIron1Active(bool status);

void setIron2Active(bool status);

void setIron1Temperature(char *temperature);

void setIron2Temperature(char *temperature);

void RS485ReceivedCommand(char RECEIVED[32]);