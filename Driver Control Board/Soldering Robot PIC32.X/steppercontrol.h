/* 
 * File:   steppercontrol.h
 * Author: andrew
 *
 * Created on 19 March 2015, 21:40
 */

void setHeadStepper1Microstepping(unsigned char steps);

char getHeadStepper2Microstepping(void);

void setHeadStepper1Current(char current);

char getHeadStepper1Current(void);


void setHeadStepper2Microstepping(unsigned char steps);

char getHeadStepper2Microstepping(void);

void setHeadStepper2Current(char current);

char getHeadStepper2Current(void);


void setSolderStepper1Microstepping(unsigned char steps);

char getSolderStepper1Microstepping(void);

void setSolderStepper1Current(char current);

char getSolderStepper1Current(void);


void setSolderStepper2Microstepping(unsigned char steps);

char getSolderStepper2Microstepping(void);

void setSolderStepper2Current(char current);

char getSolderStepper2Current(void);


void setCaddyStepperMicrostepping(unsigned char steps);

char getCaddyStepperMicrostepping(void);

void setCaddyStepperCurrent(char current);

char getCaddyStepperCurrent(void);

