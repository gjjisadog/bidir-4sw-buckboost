/*
 * Linker script for TI F28335 (GCC format)
 * Memory map based on F28335 datasheet
 */

ENTRY(ResetISR)

MEMORY
{
    /* Flash memory */
    FLASH (rx) : ORIGIN = 0x3FA000, LENGTH = 0x006000
    BEGIN (rx) : ORIGIN = 0x3F7FF6, LENGTH = 0x000002
    RESET (rx) : ORIGIN = 0x3FF5FC, LENGTH = 0x000002
    
    /* RAM memory */
    M0 (rwx) : ORIGIN = 0x000000, LENGTH = 0x000400
    M1 (rwx) : ORIGIN = 0x000400, LENGTH = 0x000400
    L0 (rwx) : ORIGIN = 0x008000, LENGTH = 0x001000
    L1 (rwx) : ORIGIN = 0x009000, LENGTH = 0x001000
    L2 (rwx) : ORIGIN = 0x00A000, LENGTH = 0x001000
    L3 (rwx) : ORIGIN = 0x00B000, LENGTH = 0x001000
    L4 (rwx) : ORIGIN = 0x00C000, LENGTH = 0x001000
    L5 (rwx) : ORIGIN = 0x00D000, LENGTH = 0x001000
    L6 (rwx) : ORIGIN = 0x00E000, LENGTH = 0x001000
    L7 (rwx) : ORIGIN = 0x00F000, LENGTH = 0x001000
    H0 (rwx) : ORIGIN = 0x200000, LENGTH = 0x002000
    H1 (rwx) : ORIGIN = 0x202000, LENGTH = 0x002000
    
    /* Peripheral memory (read-only) */
    PIE_VECT_TABLE (r) : ORIGIN = 0x000D00, LENGTH = 0x000200
    ADC_CAL (r) : ORIGIN = 0x380080, LENGTH = 0x000009
    CSM_PWL (r) : ORIGIN = 0x3F7FF8, LENGTH = 0x000008
}

/* Stack size */
__stack_size__ = 0x400;

/* Heap size for malloc */
__heap_size__ = 0x1000;

SECTIONS
{
    /* Interrupt vector table (must be at reset vector) */
    .intvecs : {
        KEEP(*(.intvecs))
    } > BEGIN

    /* Code sections */
    .text : {
        *(.text)
        *(.text.*)
        . = ALIGN(4);
    } > FLASH

    .rodata : {
        *(.rodata)
        *(.rodata.*)
        . = ALIGN(4);
    } > FLASH

    /* Initialized data */
    .data : {
        __data_start__ = .;
        *(.data)
        *(.data.*)
        . = ALIGN(4);
        __data_end__ = .;
    } > L0 AT > FLASH

    __data_load_start__ = LOADADDR(.data);

    /* Uninitialized data (BSS) */
    .bss : {
        __bss_start__ = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } > L1

    /* Stack */
    .stack : {
        . = ALIGN(8);
        . += __stack_size__;
        __stack_end__ = .;
    } > M1

    /* Heap for malloc */
    .heap : {
        . = ALIGN(4);
        __heap_start__ = .;
        . += __heap_size__;
        __heap_end__ = .;
    } > L3

    /* FreeRTOS heap (if using static allocation) */
    .freertos_heap : {
        . = ALIGN(4);
    } > L2

    /* PIE vector table (reserved, not loaded) */
    .pie_vect_table (NOLOAD) : {
        KEEP(*(.PIE_VECT_TABLE))
    } > PIE_VECT_TABLE

    /* ADC calibration (read-only, not loaded) */
    .adc_cal (NOLOAD) : {
        KEEP(*(.adc_cal))
    } > ADC_CAL

    /* CSM password (read-only, not loaded) */
    .csm_pwl (NOLOAD) : {
        KEEP(*(.csmpasswds))
    } > CSM_PWL

    /* Discard sections */
    /DISCARD/ : {
        *(.note*)
        *(.comment)
    }
}

/* Symbols for startup code */
__text_start__ = ADDR(.text);
__text_end__ = __text_start__ + SIZEOF(.text);
__data_size__ = SIZEOF(.data);
__bss_size__ = SIZEOF(.bss);
