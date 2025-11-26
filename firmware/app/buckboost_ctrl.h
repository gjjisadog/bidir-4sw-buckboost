/**
  ******************************************************************************
  * @file    buckboost_ctrl.h
  * @author  Codex
  * @brief   Control loop declarations for four-switch buck-boost.
  ******************************************************************************
  */

#ifndef _BUCKBOOST_CTRL_H_
#define _BUCKBOOST_CTRL_H_

#include <stdint.h>
#include <stdbool.h>
#include "../drivers/buckboost_hal.h"
#include "buckboost_fault.h"
#include "buckboost_state_machine.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    float kp;
    float ki;
    float integral;
    float out_min;
    float out_max;
} pi_ctrl_t;

typedef struct
{
    pi_ctrl_t  il_loop;
    pi_ctrl_t  vo_loop;
    float      vo_ref;
    float      il_ref;
    uint16_t   outer_loop_div;
    uint16_t   outer_loop_count;
    float      duty_leg_a;
    float      duty_leg_b;
    bool       buck_mode;
} buckboost_ctrl_ctx_t;

void buckboost_ctrl_init(buckboost_ctrl_ctx_t *ctx);
void buckboost_ctrl_fast_isr(buckboost_ctrl_ctx_t *ctx,
                             buckboost_hal_t *hal,
                             buckboost_meas_t *meas,
                             buckboost_fault_ctx_t *faults,
                             buckboost_state_ctx_t *sm);
void buckboost_ctrl_set_vo_ref(buckboost_ctrl_ctx_t *ctx, float vo_ref);

#ifdef __cplusplus
}
#endif

#endif /* _BUCKBOOST_CTRL_H_ */

/************************ (C) COPYRIGHT 2024 *****END OF FILE****/
