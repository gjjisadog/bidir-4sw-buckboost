/**
  ******************************************************************************
  * @file    hal_sim_stub.h
  * @brief   Helpers to feed simulated ADC data and observe PWM state.
  ******************************************************************************
  */

#ifndef _HAL_SIM_STUB_H_
#define _HAL_SIM_STUB_H_

#include <stdint.h>
#include "hal_epwm.h"

void sim_hal_set_adc_raw(uint16_t vin_raw, uint16_t vout_raw, uint16_t il_raw, uint16_t temp_raw);
HAL_epwm_cmp sim_hal_get_last_cmp(void);
int sim_hal_gate_enabled(void);

#endif /* _HAL_SIM_STUB_H_ */

/************************ (C) COPYRIGHT 2024 *****END OF FILE****/
