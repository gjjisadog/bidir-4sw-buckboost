/**
  ******************************************************************************
  * @file    buckboost_hal.c
  * @author  Codex
  * @brief   Hardware glue for four-switch buck-boost on F28335.
  ******************************************************************************
  */

#include "buckboost_hal.h"
#include "hal_bsp.h"
#include "hal_interrupt.h"

static uint16_t prv_deadtime_to_ticks(uint16_t deadtime_ns);
static void prv_configure_trip_zone(const buckboost_hal_t *hal);

void buckboost_hal_init(buckboost_hal_t *hal)
{
    uint32_t pwm_freq = BUCKBOOST_PWM_FREQUENCY_HZ;
    uint16_t deadtime_ticks = prv_deadtime_to_ticks(BUCKBOOST_DEFAULT_DEADTIME_NS);

    HAL_device_Init();
    HAL_bsp_Init();

    hal->pwm_leg_a = HAL_device_OpenByName(BUCKBOOST_PWM_LEG_A_DEV, DEVICE_FLAG_RDWR);
    hal->pwm_leg_b = HAL_device_OpenByName(BUCKBOOST_PWM_LEG_B_DEV, DEVICE_FLAG_RDWR);
    hal->adc       = HAL_device_OpenByName(BUCKBOOST_ADC_DEV,  DEVICE_FLAG_RDWR);
    hal->gate_en_gpio = HAL_device_OpenByName(BUCKBOOST_GATE_GPIO_DEV, DEVICE_FLAG_RDWR);
    hal->diag_gpio = HAL_device_OpenByName(BUCKBOOST_DIAG_GPIO_DEV, DEVICE_FLAG_RDWR);
    hal->timer_trigger = HAL_device_OpenByName(BUCKBOOST_TIMER_DEV, DEVICE_FLAG_RDWR);
    hal->watchdog  = HAL_device_OpenByName(BUCKBOOST_WATCHDOG_DEV, DEVICE_FLAG_RDWR);

    hal->pwm_period     = (uint16_t)((BUCKBOOST_SYSCLK_HZ / pwm_freq) / 2u);
    hal->adc_vin_gain   = BUCKBOOST_FROM_MICRO(BUCKBOOST_ADC_VIN_GAIN_MICRO);
    hal->adc_vout_gain  = BUCKBOOST_FROM_MICRO(BUCKBOOST_ADC_VOUT_GAIN_MICRO);
    hal->adc_il_gain    = BUCKBOOST_FROM_MICRO(BUCKBOOST_ADC_IL_GAIN_MICRO);
    hal->adc_temp_gain  = BUCKBOOST_FROM_MICRO(BUCKBOOST_ADC_TEMP_GAIN_MICRO);
    hal->adc_vin_offset  = BUCKBOOST_FROM_MICRO(BUCKBOOST_ADC_VIN_OFFSET_MICRO);
    hal->adc_vout_offset = BUCKBOOST_FROM_MICRO(BUCKBOOST_ADC_VOUT_OFFSET_MICRO);
    hal->adc_il_offset   = BUCKBOOST_FROM_MICRO(BUCKBOOST_ADC_IL_OFFSET_MICRO);
    hal->adc_temp_offset = BUCKBOOST_FROM_MICRO(BUCKBOOST_ADC_TEMP_OFFSET_MICRO);

    if (hal->pwm_leg_a)
    {
        HAL_device_Ioctl(hal->pwm_leg_a, EPWM_CTL_SET_EN_EPWM_CLK, (void *)0);
        HAL_device_Ioctl(hal->pwm_leg_a, EPWM_CTL_SET_FREQUECY, (void *)&pwm_freq);
        HAL_device_Ioctl(hal->pwm_leg_a, EPWM_CTL_SET_DEATH_TIMER, (void *)&deadtime_ticks);
    }
    if (hal->pwm_leg_b)
    {
        HAL_device_Ioctl(hal->pwm_leg_b, EPWM_CTL_SET_EN_EPWM_CLK, (void *)0);
        HAL_device_Ioctl(hal->pwm_leg_b, EPWM_CTL_SET_FREQUECY, (void *)&pwm_freq);
        HAL_device_Ioctl(hal->pwm_leg_b, EPWM_CTL_SET_DEATH_TIMER, (void *)&deadtime_ticks);
    }

    prv_configure_trip_zone(hal);

    buckboost_hal_gate_enable(hal, false);
    buckboost_hal_apply_pwm(hal, &(buckboost_pwm_cmd_t){0.0f, 0.0f, false, true});
}

void buckboost_hal_sample(const buckboost_hal_t *hal, buckboost_meas_t *meas)
{
    uint16_t raw[4] = {0};

    if (hal->adc != DEVICE_NULL)
    {
        HAL_device_Read(hal->adc, raw, 4u);
    }

    meas->vin  = (float)raw[0] * hal->adc_vin_gain  + hal->adc_vin_offset;
    meas->vout = (float)raw[1] * hal->adc_vout_gain + hal->adc_vout_offset;
    meas->il   = (float)raw[2] * hal->adc_il_gain   + hal->adc_il_offset;
    meas->temp = (float)raw[3] * hal->adc_temp_gain + hal->adc_temp_offset;
}

void buckboost_hal_apply_pwm(const buckboost_hal_t *hal, const buckboost_pwm_cmd_t *cmd)
{
    HAL_epwm_cmp cmp = {0};
    const float duty_min = BUCKBOOST_FROM_MICRO(BUCKBOOST_DUTY_MIN_MICRO);
    const float duty_max = BUCKBOOST_FROM_MICRO(BUCKBOOST_DUTY_MAX_MICRO);
    float duty_a = cmd->duty_leg_a;
    float duty_b = cmd->duty_leg_b;

    if (duty_a < duty_min) { duty_a = duty_min; }
    if (duty_a > duty_max){ duty_a = duty_max;}
    if (duty_b < duty_min) { duty_b = duty_min; }
    if (duty_b > duty_max){ duty_b = duty_max;}

    cmp.uiCmpA = (uint16_t)(duty_a * (float)hal->pwm_period);
    cmp.uiCmpB = (uint16_t)(duty_b * (float)hal->pwm_period);

    if (hal->pwm_leg_a != DEVICE_NULL)
    {
        HAL_device_Ioctl(hal->pwm_leg_a, EPWM_CTL_SET_CMPAB, (void *)&cmp);
    }
    if (hal->pwm_leg_b != DEVICE_NULL)
    {
        HAL_device_Ioctl(hal->pwm_leg_b, EPWM_CTL_SET_CMPAB, (void *)&cmp);
    }

    if (cmd->enable_gate)
    {
        buckboost_hal_gate_enable(hal, true);
        HAL_device_Ioctl(hal->pwm_leg_a, EPWM_CTL_SET_EN_EPMW_WAVE, (void *)0);
        HAL_device_Ioctl(hal->pwm_leg_b, EPWM_CTL_SET_EN_EPMW_WAVE, (void *)0);
    }
    else
    {
        HAL_device_Ioctl(hal->pwm_leg_a, EPWM_CTL_SET_DIS_EPMW_WAVE, (void *)0);
        HAL_device_Ioctl(hal->pwm_leg_b, EPWM_CTL_SET_DIS_EPMW_WAVE, (void *)0);
    }
}

void buckboost_hal_update_deadtime(const buckboost_hal_t *hal, uint16_t deadtime_ns)
{
    uint16_t deadtime_ticks = prv_deadtime_to_ticks(deadtime_ns);

    if (hal->pwm_leg_a != DEVICE_NULL)
    {
        HAL_device_Ioctl(hal->pwm_leg_a, EPWM_CTL_SET_DEATH_TIMER, (void *)&deadtime_ticks);
    }
    if (hal->pwm_leg_b != DEVICE_NULL)
    {
        HAL_device_Ioctl(hal->pwm_leg_b, EPWM_CTL_SET_DEATH_TIMER, (void *)&deadtime_ticks);
    }
}

void buckboost_hal_gate_enable(const buckboost_hal_t *hal, bool enable)
{
    hal_gpio_comm_cfg_type gpio_cfg;

    gpio_cfg.uiGpioNum = BUCKBOOST_GATE_GPIO_NUM;
    if (hal->gate_en_gpio != DEVICE_NULL)
    {
        if (enable)
        {
            HAL_device_Ioctl(hal->gate_en_gpio, GPIO_CTL_SET_ON, (void *)&gpio_cfg);
        }
        else
        {
            HAL_device_Ioctl(hal->gate_en_gpio, GPIO_CTL_SET_OFF, (void *)&gpio_cfg);
        }
    }
}

static uint16_t prv_deadtime_to_ticks(uint16_t deadtime_ns)
{
    uint32_t ticks = (uint32_t)(((uint64_t)deadtime_ns * (uint64_t)BUCKBOOST_SYSCLK_HZ) / 1000000000ULL);
    return (uint16_t)ticks;
}

static void prv_configure_trip_zone(const buckboost_hal_t *hal)
{
    /* Bind TZ1/TZ2 hardware trip to one-shot trip (OST) so PWM outputs are forced low in hardware */
    if (hal->pwm_leg_a != DEVICE_NULL)
    {
        HAL_device_Ioctl(hal->pwm_leg_a, EPWM_CTL_SET_EN_TZ_INT_OST, (void *)0);
    }
    if (hal->pwm_leg_b != DEVICE_NULL)
    {
        HAL_device_Ioctl(hal->pwm_leg_b, EPWM_CTL_SET_EN_TZ_INT_OST, (void *)0);
    }
}

/************************ (C) COPYRIGHT 2024 *****END OF FILE****/
