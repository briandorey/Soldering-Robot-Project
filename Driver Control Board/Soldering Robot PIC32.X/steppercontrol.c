/*
 *
 * Stepper management functions
 *
 * This library deals with current and microstepping for the stepper drivers
 *
 */

#ifdef __XC32
    #include <xc.h>          /* Defines special funciton registers, CP0 regs  */
#endif

#include "system.h"

#include "HardwareProfile.h"
#include "i2c.h"


#define DIGITALPOT1_ADDRESS        0x58
#define DIGITALPOT2_ADDRESS        0x5A

// Set microstepping for Header Stepper 1
// values 1, 2, 4, 8, 16, 32

void setHeadStepper1Microstepping(unsigned char steps){
    switch(steps){
        case 0x01:
            HeadStepper1_Mode0 = 0;
            HeadStepper1_Mode1 = 0;
            HeadStepper1_Mode2 = 0;
            break;
        case 0x02:
            HeadStepper1_Mode0 = 1;
            HeadStepper1_Mode1 = 0;
            HeadStepper1_Mode2 = 0;
            break;
        case 0x04:
            HeadStepper1_Mode0 = 0;
            HeadStepper1_Mode1 = 1;
            HeadStepper1_Mode2 = 0;
            break;
        case 0x08:
            HeadStepper1_Mode0 = 1;
            HeadStepper1_Mode1 = 1;
            HeadStepper1_Mode2 = 0;
            break;
        case 0x16:
            HeadStepper1_Mode0 = 0;
            HeadStepper1_Mode1 = 0;
            HeadStepper1_Mode2 = 1;
            break;
        case 0x32:
            HeadStepper1_Mode0 = 1;
            HeadStepper1_Mode1 = 0;
            HeadStepper1_Mode2 = 1;
            break;
    }
}

char getHeadStepper1Microstepping(void){
    if (HeadStepper1_Mode0 == 0 && HeadStepper1_Mode1 == 0 && HeadStepper1_Mode2 == 0){
        return 0x01;
    }
    if (HeadStepper1_Mode0 == 1 && HeadStepper1_Mode1 == 0 && HeadStepper1_Mode2 == 0){
        return 0x02;
    }
    if (HeadStepper1_Mode0 == 0 && HeadStepper1_Mode1 == 1 && HeadStepper1_Mode2 == 0){
        return 0x04;
    }
    if (HeadStepper1_Mode0 == 1 && HeadStepper1_Mode1 == 1 && HeadStepper1_Mode2 == 0){
        return 0x08;
    }
    if (HeadStepper1_Mode0 == 0 && HeadStepper1_Mode1 == 0 && HeadStepper1_Mode2 == 1){
        return 0x16;
    }
    if (HeadStepper1_Mode0 == 1 && HeadStepper1_Mode1 == 0 && HeadStepper1_Mode2 == 1){
        return 0x32;
    }
    return 1;
}


void setHeadStepper1Current(char current){
    MCP4461_SetValue(0x58, 0x03, current);
}

char getHeadStepper1Current(void){
    return MCP4461_GetValue(0x58, 0x03);
}


void setHeadStepper2Microstepping(unsigned char steps){
    switch(steps){
        case 0x01:
            HeadStepper2_Mode0 = 0;
            HeadStepper2_Mode1 = 0;
            HeadStepper2_Mode2 = 0;
            break;
        case 0x02:
            HeadStepper2_Mode0 = 1;
            HeadStepper2_Mode1 = 0;
            HeadStepper2_Mode2 = 0;
            break;
        case 0x04:
            HeadStepper2_Mode0 = 0;
            HeadStepper2_Mode1 = 1;
            HeadStepper2_Mode2 = 0;
            break;
        case 0x08:
            HeadStepper2_Mode0 = 1;
            HeadStepper2_Mode1 = 1;
            HeadStepper2_Mode2 = 0;
            break;
        case 0x16:
            HeadStepper2_Mode0 = 0;
            HeadStepper2_Mode1 = 0;
            HeadStepper2_Mode2 = 1;
            break;
        case 0x32:
            HeadStepper2_Mode0 = 1;
            HeadStepper2_Mode1 = 0;
            HeadStepper2_Mode2 = 1;
            break;
    }
}

char getHeadStepper2Microstepping(void){
    if (HeadStepper2_Mode0 == 0 && HeadStepper2_Mode1 == 0 && HeadStepper2_Mode2 == 0){
        return 0x01;
    }
    if (HeadStepper2_Mode0 == 1 && HeadStepper2_Mode1 == 0 && HeadStepper2_Mode2 == 0){
        return 0x02;
    }
    if (HeadStepper2_Mode0 == 0 && HeadStepper2_Mode1 == 1 && HeadStepper2_Mode2 == 0){
        return 0x04;
    }
    if (HeadStepper2_Mode0 == 1 && HeadStepper2_Mode1 == 1 && HeadStepper2_Mode2 == 0){
        return 0x08;
    }
    if (HeadStepper2_Mode0 == 0 && HeadStepper2_Mode1 == 0 && HeadStepper2_Mode2 == 1){
        return 0x16;
    }
    if (HeadStepper2_Mode0 == 1 && HeadStepper2_Mode1 == 0 && HeadStepper2_Mode2 == 1){
        return 0x32;
    }
    return 1;
}

void setHeadStepper2Current(char current){
    MCP4461_SetValue(0x58, 0x01, current);
}

char getHeadStepper2Current(void){
    return MCP4461_GetValue(0x58, 0x01);
}


void setSolderStepper1Microstepping(unsigned char steps){
    switch(steps){
        case 0x01:
            SolderStepper1_Mode0 = 0;
            SolderStepper1_Mode1 = 0;
            SolderStepper1_Mode2 = 0;
            break;
        case 0x02:
            SolderStepper1_Mode0 = 1;
            SolderStepper1_Mode1 = 0;
            SolderStepper1_Mode2 = 0;
            break;
        case 0x04:
            SolderStepper1_Mode0 = 0;
            SolderStepper1_Mode1 = 1;
            SolderStepper1_Mode2 = 0;
            break;
        case 0x08:
            SolderStepper1_Mode0 = 1;
            SolderStepper1_Mode1 = 1;
            SolderStepper1_Mode2 = 0;
            break;
        case 0x16:
            SolderStepper1_Mode0 = 0;
            SolderStepper1_Mode1 = 0;
            SolderStepper1_Mode2 = 1;
            break;
        case 0x32:
            SolderStepper1_Mode0 = 1;
            SolderStepper1_Mode1 = 0;
            SolderStepper1_Mode2 = 1;
            break;
    }
}

char getSolderStepper1Microstepping(void){
    if (SolderStepper1_Mode0 == 0 && SolderStepper1_Mode1 == 0 && SolderStepper1_Mode2 == 0){
        return 0x01;
    }
    if (SolderStepper1_Mode0 == 1 && SolderStepper1_Mode1 == 0 && SolderStepper1_Mode2 == 0){
        return 0x02;
    }
    if (SolderStepper1_Mode0 == 0 && SolderStepper1_Mode1 == 1 && SolderStepper1_Mode2 == 0){
        return 0x04;
    }
    if (SolderStepper1_Mode0 == 1 && SolderStepper1_Mode1 == 1 && SolderStepper1_Mode2 == 0){
        return 0x08;
    }
    if (SolderStepper1_Mode0 == 0 && SolderStepper1_Mode1 == 0 && SolderStepper1_Mode2 == 1){
        return 0x16;
    }
    if (SolderStepper1_Mode0 == 1 && SolderStepper1_Mode1 == 0 && SolderStepper1_Mode2 == 1){
        return 0x32;
    }
    return 1;
}

void setSolderStepper1Current(char current){
    MCP4461_SetValue(0x58, 0x00, current);
}

char getSolderStepper1Current(void){
    return MCP4461_GetValue(0x58, 0x00);
}


void setSolderStepper2Microstepping(unsigned char steps){
    switch(steps){
        case 0x01:
            SolderStepper2_Mode0 = 0;
            SolderStepper2_Mode1 = 0;
            SolderStepper2_Mode2 = 0;
            break;
        case 0x02:
            SolderStepper2_Mode0 = 1;
            SolderStepper2_Mode1 = 0;
            SolderStepper2_Mode2 = 0;
            break;
        case 0x04:
            SolderStepper2_Mode0 = 0;
            SolderStepper2_Mode1 = 1;
            SolderStepper2_Mode2 = 0;
            break;
        case 0x08:
            SolderStepper2_Mode0 = 1;
            SolderStepper2_Mode1 = 1;
            SolderStepper2_Mode2 = 0;
            break;
        case 0x16:
            SolderStepper2_Mode0 = 0;
            SolderStepper2_Mode1 = 0;
            SolderStepper2_Mode2 = 1;
            break;
        case 0x32:
            SolderStepper2_Mode0 = 1;
            SolderStepper2_Mode1 = 0;
            SolderStepper2_Mode2 = 1;
            break;
    }
}

char getSolderStepper2Microstepping(void){
    if (SolderStepper2_Mode0 == 0 && SolderStepper2_Mode1 == 0 && SolderStepper2_Mode2 == 0){
        return 0x01;
    }
    if (SolderStepper2_Mode0 == 1 && SolderStepper2_Mode1 == 0 && SolderStepper2_Mode2 == 0){
        return 0x02;
    }
    if (SolderStepper2_Mode0 == 0 && SolderStepper2_Mode1 == 1 && SolderStepper2_Mode2 == 0){
        return 0x04;
    }
    if (SolderStepper2_Mode0 == 1 && SolderStepper2_Mode1 == 1 && SolderStepper2_Mode2 == 0){
        return 0x08;
    }
    if (SolderStepper2_Mode0 == 0 && SolderStepper2_Mode1 == 0 && SolderStepper2_Mode2 == 1){
        return 0x16;
    }
    if (SolderStepper2_Mode0 == 1 && SolderStepper2_Mode1 == 0 && SolderStepper2_Mode2 == 1){
        return 0x32;
    }
    return 1;
}

void setSolderStepper2Current(char current){
    MCP4461_SetValue(0x5A, 0x01, current);
}

char getSolderStepper2Current(void){
    return MCP4461_GetValue(0x5A, 0x01);
}


void setCaddyStepperMicrostepping(unsigned char steps){
    switch(steps){
        case 0x01:
            CaddyStepper_Mode0 = 0;
            CaddyStepper_Mode1 = 0;
            CaddyStepper_Mode2 = 0;
            break;
        case 0x02:
            CaddyStepper_Mode0 = 1;
            CaddyStepper_Mode1 = 0;
            CaddyStepper_Mode2 = 0;
            break;
        case 0x04:
            CaddyStepper_Mode0 = 0;
            CaddyStepper_Mode1 = 1;
            CaddyStepper_Mode2 = 0;
            break;
        case 0x08:
            CaddyStepper_Mode0 = 1;
            CaddyStepper_Mode1 = 1;
            CaddyStepper_Mode2 = 0;
            break;
        case 0x16:
            CaddyStepper_Mode0 = 0;
            CaddyStepper_Mode1 = 0;
            CaddyStepper_Mode2 = 1;
            break;
        case 0x32:
            CaddyStepper_Mode0 = 1;
            CaddyStepper_Mode1 = 0;
            CaddyStepper_Mode2 = 1;
            break;
    }
}

char getCaddyStepperMicrostepping(void){
    if (SolderStepper2_Mode0 == 0 && SolderStepper2_Mode1 == 0 && SolderStepper2_Mode2 == 0){
        return 0x01;
    }
    if (SolderStepper2_Mode0 == 1 && SolderStepper2_Mode1 == 0 && SolderStepper2_Mode2 == 0){
        return 0x02;
    }
    if (SolderStepper2_Mode0 == 0 && SolderStepper2_Mode1 == 1 && SolderStepper2_Mode2 == 0){
        return 0x04;
    }
    if (SolderStepper2_Mode0 == 1 && SolderStepper2_Mode1 == 1 && SolderStepper2_Mode2 == 0){
        return 0x08;
    }
    if (SolderStepper2_Mode0 == 0 && SolderStepper2_Mode1 == 0 && SolderStepper2_Mode2 == 1){
        return 0x16;
    }
    if (SolderStepper2_Mode0 == 1 && SolderStepper2_Mode1 == 0 && SolderStepper2_Mode2 == 1){
        return 0x32;
    }
    return 1;
}

void setCaddyStepperCurrent(char current){
    MCP4461_SetValue(0x5A, 0x00, current);
}

char getCaddyStepperCurrent(void){
    return MCP4461_GetValue(0x5A, 0x00);
}