/**
  ******************************************************************************
  * @file    buckboost_fault.c
  * @author  Codex
  * @brief   Fault detection and latch logic.
  ******************************************************************************
  */

#include "buckboost_fault.h"
#include "../bsp/buckboost_f28335_config.h"

void buckboost_fault_init(buckboost_fault_ctx_t *ctx)
{
    buckboost_fault_threshold_t th =
    {
        .vin_ov    = BUCKBOOST_FROM_MICRO(BUCKBOOST_VIN_OV_LIMIT_MICRO),
        .vin_uv    = BUCKBOOST_FROM_MICRO(BUCKBOOST_VIN_UV_LIMIT_MICRO),
        .vout_ov   = BUCKBOOST_FROM_MICRO(BUCKBOOST_VOUT_OV_LIMIT_MICRO),
        .vout_uv   = BUCKBOOST_FROM_MICRO(BUCKBOOST_VOUT_UV_LIMIT_MICRO),
        .il_oc     = BUCKBOOST_FROM_MICRO(BUCKBOOST_IL_OC_LIMIT_MICRO),
        .temp_ot   = BUCKBOOST_FROM_MICRO(BUCKBOOST_TEMP_OT_LIMIT_MICRO),
        .adc_margin= BUCKBOOST_FROM_MICRO(BUCKBOOST_ADC_MARGIN_MICRO)
    };
    ctx->last = BUCKBOOST_FAULT_NONE;
    ctx->latched = false;
    ctx->th = th;
}

void buckboost_fault_configure(buckboost_fault_ctx_t *ctx, const buckboost_fault_threshold_t *th)
{
    ctx->th = *th;
}

void buckboost_fault_clear(buckboost_fault_ctx_t *ctx)
{
    ctx->latched = false;
    ctx->last = BUCKBOOST_FAULT_NONE;
}

bool buckboost_fault_check(buckboost_fault_ctx_t *ctx, float vin, float vout, float il, float temp)
{
    buckboost_fault_id_t fault = BUCKBOOST_FAULT_NONE;

    if (vin > ctx->th.vin_ov)      { fault = BUCKBOOST_FAULT_VIN_OV; }
    else if (vin < ctx->th.vin_uv) { fault = BUCKBOOST_FAULT_VIN_UV; }
    else if (vout > ctx->th.vout_ov){ fault = BUCKBOOST_FAULT_VOUT_OV; }
    else if (vout < ctx->th.vout_uv){ fault = BUCKBOOST_FAULT_VOUT_UV; }
    else if (il > ctx->th.il_oc)   { fault = BUCKBOOST_FAULT_IL_OC; }
    else if (temp > ctx->th.temp_ot){ fault = BUCKBOOST_FAULT_TEMP_OT; }

    if (fault != BUCKBOOST_FAULT_NONE)
    {
        ctx->last = fault;
        ctx->latched = true;
    }

    return ctx->latched;
}

/************************ (C) COPYRIGHT 2024 *****END OF FILE****/
