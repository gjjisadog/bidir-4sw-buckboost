/**
  ******************************************************************************
  * @file    buckboost_f28335_config.h
  * @author  Codex
  * @brief   Integer-based defaults and device mapping for F28335 target.
  ******************************************************************************
  */

#ifndef _BUCKBOOST_F28335_CONFIG_H_
#define _BUCKBOOST_F28335_CONFIG_H_

#include <stdint.h>
#include "hal_device.h"

/* Fixed-point helpers so configuration values stay in integer form */
#define BUCKBOOST_FIXED_POINT_MICRO      (1000000L)
#define BUCKBOOST_FIXED_POINT_MILLI      (1000L)
#define BUCKBOOST_FROM_MICRO(x)          ((float)(x) / (float)BUCKBOOST_FIXED_POINT_MICRO)
#define BUCKBOOST_FROM_MILLI(x)          ((float)(x) / (float)BUCKBOOST_FIXED_POINT_MILLI)

/* Core clocks / timing for TMS320F28335 */
#define BUCKBOOST_F28335_SYSCLK_HZ       (150000000UL)
#define BUCKBOOST_F28335_PWM_FREQ_HZ     (20000UL)
#define BUCKBOOST_F28335_DEADTIME_NS     (500UL)

/* Hardware resource mapping on F28335 */
#define BUCKBOOST_PWM_LEG_A_DEV          DEVICE_NAME_EPWM1
#define BUCKBOOST_PWM_LEG_B_DEV          DEVICE_NAME_EPWM2
#define BUCKBOOST_ADC_DEV                DEVICE_NAME_ADCA
#define BUCKBOOST_GATE_GPIO_DEV          DEVICE_NAME_GPIO
#define BUCKBOOST_DIAG_GPIO_DEV          DEVICE_NAME_BOARD_PROBE
#define BUCKBOOST_TIMER_DEV              DEVICE_NAME_TIMER0
#define BUCKBOOST_WATCHDOG_DEV           DEVICE_NAME_WATCHDOG
#define BUCKBOOST_GATE_GPIO_NUM          (0u)

/* ADC gains/offsets kept as integers (micro-units) */
#define BUCKBOOST_ADC_VIN_GAIN_MICRO     (10000L)     /* 0.01 */
#define BUCKBOOST_ADC_VOUT_GAIN_MICRO    (10000L)
#define BUCKBOOST_ADC_IL_GAIN_MICRO      (10000L)
#define BUCKBOOST_ADC_TEMP_GAIN_MICRO    (10000L)
#define BUCKBOOST_ADC_VIN_OFFSET_MICRO   (0L)
#define BUCKBOOST_ADC_VOUT_OFFSET_MICRO  (0L)
#define BUCKBOOST_ADC_IL_OFFSET_MICRO    (0L)
#define BUCKBOOST_ADC_TEMP_OFFSET_MICRO  (0L)

/* PI parameters (micro scale to retain precision) */
#define BUCKBOOST_IL_KP_MICRO            (20000L)         /* 0.02 */
#define BUCKBOOST_IL_KI_MICRO            (150000000L)     /* 150.0 */
#define BUCKBOOST_IL_OUT_MIN_MICRO       (20000L)         /* 0.02 */
#define BUCKBOOST_IL_OUT_MAX_MICRO       (950000L)        /* 0.95 */

#define BUCKBOOST_VO_KP_MICRO            (50000L)         /* 0.05 */
#define BUCKBOOST_VO_KI_MICRO            (30000000L)      /* 30.0 */
#define BUCKBOOST_VO_OUT_MIN_MICRO       (0L)             /* 0.0 */
#define BUCKBOOST_VO_OUT_MAX_MICRO       (50000000L)      /* 50.0 */

#define BUCKBOOST_DEFAULT_VO_REF_MICRO   (400000000L)     /* 400.0 */
#define BUCKBOOST_VO_REF_MIN_MICRO       (20000000L)      /* 20.0 */
#define BUCKBOOST_VO_REF_MAX_MICRO       (800000000L)     /* 800.0 */
#define BUCKBOOST_IL_REF_MIN_MICRO       (-40000000L)     /* -40.0 */
#define BUCKBOOST_IL_REF_MAX_MICRO       (40000000L)      /* 40.0 */

/* Duty clamp in micro (per-unit x1e6) */
#define BUCKBOOST_DUTY_MIN_MICRO         (0L)
#define BUCKBOOST_DUTY_MAX_MICRO         (950000L)        /* 0.95 */

/* Fault thresholds (micro scale) */
#define BUCKBOOST_VIN_OV_LIMIT_MICRO     (32000000L)      /* 32.0 V soft OVP */
#define BUCKBOOST_VIN_UV_LIMIT_MICRO     (8000000L)       /* 8.0 V soft UVP */
#define BUCKBOOST_VOUT_OV_LIMIT_MICRO    (1000000000L)    /* placeholder, adjust per design */
#define BUCKBOOST_VOUT_UV_LIMIT_MICRO    (50000000L)      /* placeholder, adjust per design */
#define BUCKBOOST_IL_OC_LIMIT_MICRO      (10000000L)      /* 10.0 A soft OCP */
#define BUCKBOOST_TEMP_OT_LIMIT_MICRO    (100000000L)     /* 100.0 C */
#define BUCKBOOST_ADC_MARGIN_MICRO       (50000L)         /* 0.05 */

#endif /* _BUCKBOOST_F28335_CONFIG_H_ */

/************************ (C) COPYRIGHT 2024 *****END OF FILE****/
