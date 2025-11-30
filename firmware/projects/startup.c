/**
  ******************************************************************************
  * @file    startup.c
  * @brief   Startup code and interrupt vector table for F28335.
  ******************************************************************************
  */

#include <stdint.h>

/* External symbols */
extern void main(void);
extern void SystemInit(void);

/* Reset interrupt handler */
void ResetISR(void) __attribute__((interrupt));
void ResetISR(void)
{
    /* Initialize system */
    SystemInit();
    
    /* Call main */
    main();
    
    /* Should never return */
    while(1);
}

/* Default interrupt handler */
void DefaultISR(void) __attribute__((interrupt));
void DefaultISR(void)
{
    /* Infinite loop */
    while(1);
}

/* System initialization */
void SystemInit(void)
{
    /* Disable watchdog */
    volatile uint16_t *WDCR = (volatile uint16_t *)0x00C19;
    *WDCR = 0x0068;  /* Disable watchdog */
    
    /* Initialize PLL if needed */
    /* Add your system initialization code here */
}

/* Interrupt vector table */
typedef void (*pfnISR)(void);

/* Place interrupt vector table in a dedicated section */
__attribute__((section(".intvecs")))
pfnISR intvecs[] = {
    (pfnISR)&ResetISR,      /* Reset */
    DefaultISR,             /* INT1 */
    DefaultISR,             /* INT2 */
    DefaultISR,             /* INT3 */
    DefaultISR,             /* INT4 */
    DefaultISR,             /* INT5 */
    DefaultISR,             /* INT6 */
    DefaultISR,             /* INT7 */
    DefaultISR,             /* INT8 */
    DefaultISR,             /* INT9 */
    DefaultISR,             /* INT10 */
    DefaultISR,             /* INT11 */
    DefaultISR,             /* INT12 */
    DefaultISR,             /* INT13 */
    DefaultISR,             /* INT14 */
    DefaultISR,             /* DLOGINT */
    DefaultISR,             /* RTOSINT */
    DefaultISR,             /* EMUINT */
    DefaultISR,             /* NMI */
    DefaultISR,             /* ILLEGAL */
    DefaultISR,             /* USER1 */
    DefaultISR,             /* USER2 */
    DefaultISR,             /* USER3 */
    DefaultISR,             /* USER4 */
    DefaultISR,             /* USER5 */
    DefaultISR,             /* USER6 */
    DefaultISR,             /* USER7 */
    DefaultISR,             /* USER8 */
    DefaultISR,             /* USER9 */
    DefaultISR,             /* USER10 */
    DefaultISR,             /* USER11 */
    DefaultISR,             /* USER12 */
};

