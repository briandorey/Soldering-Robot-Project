/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#ifdef __XC32
    #include <xc.h>          /* Defines special funciton registers, CP0 regs  */
#endif

#include <plib.h>            /* Include to use PIC32 peripheral libraries     */
#include <stdint.h>          /* For uint32_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */
#include "system.h"          /* variables/params used by system.c             */

/******************************************************************************/
/* System Level Functions                                                     */
/*                                                                            */
/* Custom oscillator configuration funtions, reset source evaluation          */
/* functions, and other non-peripheral microcontroller initialization         */
/* functions get placed in system.c                                           */
/*                                                                            */
/******************************************************************************/

/* <Initialize variables in system.h and put code for system code here.>      */
// prototype
void DelayMs(unsigned int);
void DelayUs(unsigned int);

#define msPeriod 30000
#define usPeriod 30
 
void DelayMs(unsigned int msec)
 {
     unsigned int tWait, tStart;

     tWait=(msPeriod)*msec;        //    SYS_FREQ        (60000000) 60MHz
     tStart=ReadCoreTimer();
     while((ReadCoreTimer()-tStart)<tWait);        // wait for the time to pass

 }

void DelayUs(unsigned int usec)
 {
     unsigned int tWait, tStart;

     tWait=(usPeriod)*usec;        //    SYS_FREQ        (60000000) 60MHz
     tStart=ReadCoreTimer();
     while((ReadCoreTimer()-tStart)<tWait);        // wait for the time to pass

 }