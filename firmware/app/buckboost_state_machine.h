/**
  ******************************************************************************
  * @file    buckboost_state_machine.h
  * @author  Codex
  * @brief   Run/stop state machine.
  ******************************************************************************
  */

#ifndef _BUCKBOOST_STATE_MACHINE_H_
#define _BUCKBOOST_STATE_MACHINE_H_

#include <stdint.h>
#include <stdbool.h>
#include "buckboost_fault.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    BUCKBOOST_STATE_INIT = 0,
    BUCKBOOST_STATE_PRECHARGE,
    BUCKBOOST_STATE_SOFTSTART,
    BUCKBOOST_STATE_RUN,
    BUCKBOOST_STATE_FAULT
} buckboost_state_t;

typedef struct
{
    buckboost_state_t state;
    uint32_t          softstart_ticks;
    uint32_t          precharge_ticks;
    buckboost_fault_id_t fault_reason;
} buckboost_state_ctx_t;

void buckboost_state_init(buckboost_state_ctx_t *ctx);
void buckboost_state_step(buckboost_state_ctx_t *ctx, bool fault_active);
bool buckboost_state_is_running(const buckboost_state_ctx_t *ctx);

#ifdef __cplusplus
}
#endif

#endif /* _BUCKBOOST_STATE_MACHINE_H_ */

/************************ (C) COPYRIGHT 2024 *****END OF FILE****/
