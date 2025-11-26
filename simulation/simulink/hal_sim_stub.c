/**
  ******************************************************************************
  * @file    hal_sim_stub.c
  * @brief   Stub implementations of HAL entry points for MEX/Simulink.
  ******************************************************************************
  */

#include <string.h>
#include <stdbool.h>
#include "hal_device.h"
#include "hal_epwm.h"
#include "hal_gpio.h"
#include "hal_sim_stub.h"

static uint16_t g_sim_adc_raw[4] = {0};
static HAL_epwm_cmp g_last_cmp = {0};
static bool g_gate_on = false;

void HAL_device_Init(void)
{
    /* No hardware init in simulation */
}

hal_device_t HAL_device_OpenByName(const char *name, uint32_t flags)
{
    (void)name;
    (void)flags;
    return (hal_device_t)1;
}

int HAL_device_Ioctl(hal_device_t dev, uint32_t cmd, void *param)
{
    (void)dev;
    switch (cmd)
    {
        case EPWM_CTL_SET_CMPAB:
            if (param != NULL)
            {
                memcpy(&g_last_cmp, param, sizeof(HAL_epwm_cmp));
            }
            break;
        case EPWM_CTL_SET_EN_EPMW_WAVE:
            g_gate_on = true;
            break;
        case EPWM_CTL_SET_DIS_EPMW_WAVE:
            g_gate_on = false;
            break;
        default:
            /* Other controls ignored in simulation */
            break;
    }
    return 0;
}

int HAL_device_Read(hal_device_t dev, void *buffer, uint32_t len)
{
    (void)dev;
    uint32_t words = len;
    uint16_t *dst = (uint16_t *)buffer;
    for (uint32_t i = 0u; i < words && i < 4u; ++i)
    {
        dst[i] = g_sim_adc_raw[i];
    }
    return (int)words;
}

int HAL_device_Write(hal_device_t dev, const void *buffer, uint32_t len)
{
    (void)dev;
    (void)buffer;
    (void)len;
    return 0;
}

void sim_hal_set_adc_raw(uint16_t vin_raw, uint16_t vout_raw, uint16_t il_raw, uint16_t temp_raw)
{
    g_sim_adc_raw[0] = vin_raw;
    g_sim_adc_raw[1] = vout_raw;
    g_sim_adc_raw[2] = il_raw;
    g_sim_adc_raw[3] = temp_raw;
}

HAL_epwm_cmp sim_hal_get_last_cmp(void)
{
    return g_last_cmp;
}

int sim_hal_gate_enabled(void)
{
    return g_gate_on ? 1 : 0;
}

/************************ (C) COPYRIGHT 2024 *****END OF FILE****/
