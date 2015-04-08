/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#ifdef __XC32
    #include <xc.h>          /* Defines special funciton registers, CP0 regs  */
#endif

#include <plib.h>            /* Include to use PIC32 peripheral libraries     */
#include <stdint.h>          /* For uint32_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */

/******************************************************************************/
/* Exception Macro Definitions                                                */
/******************************************************************************/

/*When WRITE_EXCEPTION_CAUSE_TO_FLASH is defined the PIC32 executes a self
write routine to save the exception cause register.*/

/* #define WRITE_EXCEPTION_CAUSE_TO_FLASH */

#ifdef WRITE_EXCEPTION_CAUSE_TO_FLASH

/* Physical Addresses which are at the end of KSEG 0 program memory. */
/* User may want to adjust these values */
#define EXCEPTION_CAUSE 0x1D007FFC
#define EXCEPTION_ADDR  0x1D007FF8

#endif

/******************************************************************************/
/* Exception Variable Declaration                                             */
/******************************************************************************/

/* static in case exception condition would stop auto variable being created  */
static enum {
    EXCEP_IRQ = 0,           /* interrupt */
    EXCEP_AdEL = 4,          /* address error exception (load or ifetch) */
    EXCEP_AdES,              /* address error exception (store) */
    EXCEP_IBE,               /* bus error (ifetch) */
    EXCEP_DBE,               /* bus error (load/store) */
    EXCEP_Sys,               /* syscall */
    EXCEP_Bp,                /* breakpoint */
    EXCEP_RI,                /* reserved instruction */
    EXCEP_CpU,               /* coprocessor unusable */
    EXCEP_Overflow,          /* arithmetic overflow */
    EXCEP_Trap,              /* trap (possible divide by zero) */
    EXCEP_IS1 = 16,          /* implementation specfic 1 */
    EXCEP_CEU,               /* CorExtend Unuseable */
    EXCEP_C2E                /* coprocessor 2 */
} _excep_code;

/* static in case exception condition would stop auto variable being created */
static unsigned int _epc_code;
static unsigned int _excep_addr;

/******************************************************************************/
/* Exception Handling                                                         */
/******************************************************************************/

/* This function overrides the normal _weak_ _generic_exception_handler which
is defined in the C32 User's Guide.  The _weak_ _generic_exception_handler
just does an infinite loop. */
void _general_exception_handler(void)
{
    unsigned long t0 = _CP0_GET_COUNT(); /* Used for NVMOP 6 us Delay */

    /* Mask off Mask of the ExcCode Field from the Cause Register
    Refer to the MIPs M4K Software User's manual */
    _excep_code=_CP0_GET_CAUSE() & 0x0000007C >> 2;
    _excep_addr=_CP0_GET_EPC();

    _CP0_SET_STATUS(_CP0_GET_STATUS()&0xFFFFFFE); /* Disable Interrupts */

#ifdef WRITE_EXCEPTION_CAUSE_TO_FLASH

    /* Store the exception causes in program memory in case the part exhibited
    the problem in release mode.  Gives user a place to start debugging
    the problem. */

    NVMCON = 0x4001;            /* set WREN and Word Programing mode */
    NVMADDR = EXCEPTION_CAUSE;  /* PM Address at which we'll store the */
                                /* cause register */
    NVMDATA   = _excep_code;

    /* wait at least 6 us for LVD start-up
    assume we're running at max frequency
    (80 MHz) so we're always safe */
    {
        while (_CP0_GET_COUNT() - t0 < (80/2)*6);
    }

    NVMKEY    = 0xAA996655;
    NVMKEY    = 0x556699AA;     /* unlock sequence */
    NVMCONSET = NVMCON_WR;
    while(NVMCON & NVMCON_WR);  /* wait on write to finish */

    NVMCON = 0x4001;            /* set WREN and Word Programing mode */
    NVMADDR = EXCEPTION_ADDR;   /* PM Address at which we'll store the */
                                /* exception address register */
    NVMDATA   = _excep_addr;

    /* wait at least 6 us for LVD start-up
    assume we're running at max frequency
    (80 MHz) so we're always safe */
    {
        while (_CP0_GET_COUNT() - t0 < (80/2)*6);
    }

    NVMKEY    = 0xAA996655;
    NVMKEY    = 0x556699AA;     /* unlock sequence */
    NVMCONSET = NVMCON_WR;
    while(NVMCON & NVMCON_WR);

    /* Write the exception cause and address to the part can be read and
    the cause determined. */
    NVMWriteWord((void*)EXCEPTION_CAUSE, _excep_code);
    NVMWriteWord((void*)EXCEPTION_ADDR, _excep_addr);

#endif

    while (1)
    {
        /* Examine _excep_code to identify the type of exception */
        /* Examine _excep_addr to find the address that caused the exception */
    }
}
