/**
  ******************************************************************************
  * @file    buckboost_hal.h
  * @author  Codex
  * @brief   Hardware abstraction for four-switch buck-boost on F28335.
  ******************************************************************************
  */

#ifndef _BUCKBOOST_HAL_H_
#define _BUCKBOOST_HAL_H_

#include <stdint.h>
#include <stdbool.h>
#include "hal_device.h"
#include "hal_adc.h"
#include "hal_epwm.h"
#include "hal_gpio.h"
#include "hal_timer.h"
#include "hal_watchdog.h"
#include "../bsp/buckboost_f28335_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BUCKBOOST_PWM_FREQUENCY_HZ
#define BUCKBOOST_PWM_FREQUENCY_HZ        (BUCKBOOST_F28335_PWM_FREQ_HZ)
#endif
#ifndef BUCKBOOST_DEFAULT_DEADTIME_NS
#define BUCKBOOST_DEFAULT_DEADTIME_NS     (BUCKBOOST_F28335_DEADTIME_NS)
#endif
#ifndef BUCKBOOST_SYSCLK_HZ
#define BUCKBOOST_SYSCLK_HZ               (BUCKBOOST_F28335_SYSCLK_HZ)
#endif

typedef struct
{
    float vin;
    float vout;
    float il;
    float temp;
} buckboost_meas_t;

typedef struct
{
    float duty_leg_a;
    float duty_leg_b;
    bool  enable_gate;
    bool  buck_mode;
} buckboost_pwm_cmd_t;

typedef struct
{
    hal_device_t pwm_leg_a;
    hal_device_t pwm_leg_b;
    hal_device_t adc;
    hal_device_t gate_en_gpio;
    hal_device_t diag_gpio;
    hal_device_t timer_trigger;
    hal_device_t watchdog;
    uint16_t     pwm_period;
    float        adc_vin_gain;
    float        adc_vout_gain;
    float        adc_il_gain;
    float        adc_temp_gain;
    float        adc_vin_offset;
    float        adc_vout_offset;
    float        adc_il_offset;
    float        adc_temp_offset;
} buckboost_hal_t;

void buckboost_hal_init(buckboost_hal_t *hal);
void buckboost_hal_sample(const buckboost_hal_t *hal, buckboost_meas_t *meas);
void buckboost_hal_apply_pwm(const buckboost_hal_t *hal, const buckboost_pwm_cmd_t *cmd);
void buckboost_hal_update_deadtime(const buckboost_hal_t *hal, uint16_t deadtime_ns);
void buckboost_hal_gate_enable(const buckboost_hal_t *hal, bool enable);

#ifdef __cplusplus
}
#endif

#endif /* _BUCKBOOST_HAL_H_ */

/************************ (C) COPYRIGHT 2024 *****END OF FILE****/
