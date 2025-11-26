/**
  ******************************************************************************
  * @file    main.c
  * @author  Codex
  * @brief   Entry for four-switch buck-boost on F28335 (20 kHz).
  ******************************************************************************
  */

#include "../drivers/buckboost_hal.h"
#include "buckboost_ctrl.h"
#include "buckboost_fault.h"
#include "buckboost_state_machine.h"
#include "hal_interrupt.h"
#include "hal_watchdog.h"
#include "hal_bsp.h"
#include "FreeRTOS.h"
#include "task.h"

static buckboost_hal_t g_hal;
static buckboost_ctrl_ctx_t g_ctrl;
static buckboost_fault_ctx_t g_fault;
static buckboost_state_ctx_t g_sm;
static buckboost_meas_t g_meas;

static void prv_housekeeping(void *argument);

#ifdef __TI_COMPILER_VERSION__
interrupt void buckboost_epwm_isr(void);
#else
void buckboost_epwm_isr(void);
#endif

int16_t main(void)
{
    buckboost_hal_init(&g_hal);
    buckboost_fault_init(&g_fault);
    buckboost_state_init(&g_sm);
    buckboost_ctrl_init(&g_ctrl);

    /* Register fast ISR to ePWM1 interrupt so control runs at 20 kHz */
    if (g_hal.pwm_leg_a != DEVICE_NULL)
    {
        irq_fn_t isr = &buckboost_epwm_isr;
        HAL_device_Ioctl(g_hal.pwm_leg_a, EPWM_CTL_SET_APPINTERRUPT, (void *)&isr);
        HAL_device_Ioctl(g_hal.pwm_leg_a, EPWM_CTL_SET_EN_TZ_INT_OST, (void *)0);
    }

    xTaskCreate(prv_housekeeping, "app_logic", 512u, NULL, tskIDLE_PRIORITY + 2u, NULL);
    vTaskStartScheduler();

    for (;;)
    {
    }
}

static void prv_housekeeping(void *argument)
{
    argument = argument;
    for (;;)
    {
        /* Slow tasks: watchdog, setpoint update, telemetry hooks */
        if (g_hal.watchdog != DEVICE_NULL)
        {
            uint16_t feed = WDG_FOOD_0X55;
            HAL_device_Ioctl(g_hal.watchdog, WDG_CTL_SET_FEED, (void *)&feed);
        }

        vTaskDelay(10u / portTICK_PERIOD_MS);
    }
}

void buckboost_epwm_isr(void)
{
    buckboost_ctrl_fast_isr(&g_ctrl, &g_hal, &g_meas, &g_fault, &g_sm);
    /* Interrupt flag clear is device-specific; handled inside driver layer. */
}

/************************ (C) COPYRIGHT 2024 *****END OF FILE****/
