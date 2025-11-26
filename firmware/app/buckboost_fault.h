/**
  ******************************************************************************
  * @file    buckboost_fault.h
  * @author  Codex
  * @brief   Fault thresholds and latch logic.
  ******************************************************************************
  */

#ifndef _BUCKBOOST_FAULT_H_
#define _BUCKBOOST_FAULT_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    BUCKBOOST_FAULT_NONE = 0,
    BUCKBOOST_FAULT_VIN_OV,
    BUCKBOOST_FAULT_VIN_UV,
    BUCKBOOST_FAULT_VOUT_OV,
    BUCKBOOST_FAULT_VOUT_UV,
    BUCKBOOST_FAULT_IL_OC,
    BUCKBOOST_FAULT_TEMP_OT,
    BUCKBOOST_FAULT_ADC_RANGE,
    BUCKBOOST_FAULT_GATE_DRV,
    BUCKBOOST_FAULT_UNKNOWN
} buckboost_fault_id_t;

typedef struct
{
    float vin_ov;
    float vin_uv;
    float vout_ov;
    float vout_uv;
    float il_oc;
    float temp_ot;
    float adc_margin;
} buckboost_fault_threshold_t;

typedef struct
{
    buckboost_fault_id_t last;
    bool                 latched;
    buckboost_fault_threshold_t th;
} buckboost_fault_ctx_t;

void buckboost_fault_init(buckboost_fault_ctx_t *ctx);
void buckboost_fault_configure(buckboost_fault_ctx_t *ctx, const buckboost_fault_threshold_t *th);
void buckboost_fault_clear(buckboost_fault_ctx_t *ctx);
bool buckboost_fault_check(buckboost_fault_ctx_t *ctx, float vin, float vout, float il, float temp);

#ifdef __cplusplus
}
#endif

#endif /* _BUCKBOOST_FAULT_H_ */

/************************ (C) COPYRIGHT 2024 *****END OF FILE****/
