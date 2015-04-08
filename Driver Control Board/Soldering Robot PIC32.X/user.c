/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#ifdef __XC32
    #include <xc.h>          /* Defines special funciton registers, CP0 regs  */
#endif

#include <plib.h>            /* Include to use PIC32 peripheral libraries     */
#include <stdint.h>          /* For uint32_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */
#include "user.h"            /* variables/params used by user.c               */

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* TODO Initialize User Ports/Peripherals/Project here */

void InitApp(void)
{
    // Disable JTAG debugging
    DDPCON = 0;

    /* Setup analog functionality and port direction */

    // Set all analog pins to be digital I/O
    AD1PCFG = 0xFFFF;

    /* Initialize IO */

    TRISA = 0x0000;
    TRISB = 0x0000;
    TRISC = 0x0006;
    TRISD = 0x300e;
    TRISE = 0x0000;
    TRISF = 0x0108;
    TRISG = 0x028c;

    LATA = 0x0000;
    LATB = 0x0000;
    LATC = 0x0000;
    LATD = 0x0000;
    LATE = 0x0000;

    
    /* Initialize Bluetooth */



    /* Initialize Debug */

    /* Initialize RS485 */

    /* Initialize i2c */
}

