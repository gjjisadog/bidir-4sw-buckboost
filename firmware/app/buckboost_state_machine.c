/**
  ******************************************************************************
  * @file    buckboost_state_machine.c
  * @author  Codex
  * @brief   Simple run/stop state handling.
  ******************************************************************************
  */

#include "buckboost_state_machine.h"

void buckboost_state_init(buckboost_state_ctx_t *ctx)
{
    ctx->state = BUCKBOOST_STATE_INIT;
    ctx->softstart_ticks = 0u;
    ctx->precharge_ticks = 0u;
    ctx->fault_reason = BUCKBOOST_FAULT_NONE;
}

void buckboost_state_step(buckboost_state_ctx_t *ctx, bool fault_active)
{
    switch (ctx->state)
    {
        case BUCKBOOST_STATE_INIT:
            ctx->precharge_ticks++;
            if (ctx->precharge_ticks > 400u)
            {
                ctx->state = BUCKBOOST_STATE_PRECHARGE;
            }
            break;
        case BUCKBOOST_STATE_PRECHARGE:
            ctx->precharge_ticks++;
            if (fault_active)
            {
                ctx->state = BUCKBOOST_STATE_FAULT;
                ctx->fault_reason = BUCKBOOST_FAULT_UNKNOWN;
            }
            else if (ctx->precharge_ticks > 1000u)
            {
                ctx->state = BUCKBOOST_STATE_SOFTSTART;
                ctx->softstart_ticks = 0u;
            }
            break;
        case BUCKBOOST_STATE_SOFTSTART:
            ctx->softstart_ticks++;
            if (fault_active)
            {
                ctx->state = BUCKBOOST_STATE_FAULT;
                ctx->fault_reason = BUCKBOOST_FAULT_UNKNOWN;
            }
            else if (ctx->softstart_ticks > 2000u)
            {
                ctx->state = BUCKBOOST_STATE_RUN;
            }
            break;
        case BUCKBOOST_STATE_RUN:
            if (fault_active)
            {
                ctx->state = BUCKBOOST_STATE_FAULT;
            }
            break;
        case BUCKBOOST_STATE_FAULT:
        default:
            ctx->state = BUCKBOOST_STATE_FAULT;
            break;
    }
}

bool buckboost_state_is_running(const buckboost_state_ctx_t *ctx)
{
    return (ctx->state == BUCKBOOST_STATE_RUN || ctx->state == BUCKBOOST_STATE_SOFTSTART);
}


/************************ (C) COPYRIGHT 2024 *****END OF FILE****/
