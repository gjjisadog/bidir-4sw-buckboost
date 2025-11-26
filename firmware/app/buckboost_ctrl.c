/**
  ******************************************************************************
  * @file    buckboost_ctrl.c
  * @author  Codex
  * @brief   Control ISR and PI loops for four-switch buck-boost.
  ******************************************************************************
  */

#include "buckboost_ctrl.h"
#include "hal_watchdog.h"
#include <math.h>

static float pi_run(pi_ctrl_t *pi, float err, float Ts);

void buckboost_ctrl_init(buckboost_ctrl_ctx_t *ctx)
{
    ctx->il_loop.kp = BUCKBOOST_FROM_MICRO(BUCKBOOST_IL_KP_MICRO);
    ctx->il_loop.ki = BUCKBOOST_FROM_MICRO(BUCKBOOST_IL_KI_MICRO);
    ctx->il_loop.integral = 0.0f;
    ctx->il_loop.out_min = BUCKBOOST_FROM_MICRO(BUCKBOOST_IL_OUT_MIN_MICRO);
    ctx->il_loop.out_max = BUCKBOOST_FROM_MICRO(BUCKBOOST_IL_OUT_MAX_MICRO);

    ctx->vo_loop.kp = BUCKBOOST_FROM_MICRO(BUCKBOOST_VO_KP_MICRO);
    ctx->vo_loop.ki = BUCKBOOST_FROM_MICRO(BUCKBOOST_VO_KI_MICRO);
    ctx->vo_loop.integral = 0.0f;
    ctx->vo_loop.out_min = BUCKBOOST_FROM_MICRO(BUCKBOOST_VO_OUT_MIN_MICRO);
    ctx->vo_loop.out_max = BUCKBOOST_FROM_MICRO(BUCKBOOST_VO_OUT_MAX_MICRO); /* A ref limit */

    ctx->vo_ref = BUCKBOOST_FROM_MICRO(BUCKBOOST_DEFAULT_VO_REF_MICRO);
    ctx->il_ref = 0.0f;
    ctx->outer_loop_div = (uint16_t)(BUCKBOOST_PWM_FREQUENCY_HZ / 1000u);
    if (ctx->outer_loop_div == 0u) { ctx->outer_loop_div = 1u; }
    ctx->outer_loop_count = 0u;
    ctx->duty_leg_a = 0.0f;
    ctx->duty_leg_b = 0.0f;
    ctx->buck_mode = true;
}

void buckboost_ctrl_fast_isr(buckboost_ctrl_ctx_t *ctx,
                             buckboost_hal_t *hal,
                             buckboost_meas_t *meas,
                             buckboost_fault_ctx_t *faults,
                             buckboost_state_ctx_t *sm)
{
    float Ts = 1.0f / (float)BUCKBOOST_PWM_FREQUENCY_HZ;
    buckboost_pwm_cmd_t cmd = {0};
    bool fault = false;

    buckboost_hal_sample(hal, meas);
    fault = buckboost_fault_check(faults, meas->vin, meas->vout, meas->il, meas->temp);

    /* Soft protection: immediately stop PWM if ADC values are out of bounds */
    if (fault)
    {
        buckboost_state_step(sm, true);
        cmd.enable_gate = false;
        cmd.duty_leg_a = 0.0f;
        cmd.duty_leg_b = 0.0f;
        buckboost_hal_gate_enable(hal, false);
        buckboost_hal_apply_pwm(hal, &cmd);
        return;
    }

    buckboost_state_step(sm, false);

    cmd.enable_gate = buckboost_state_is_running(sm) && !fault;
    if (!cmd.enable_gate)
    {
        cmd.duty_leg_a = 0.0f;
        cmd.duty_leg_b = 0.0f;
        buckboost_hal_gate_enable(hal, false);
        buckboost_hal_apply_pwm(hal, &cmd);
        return;
    }

    ctx->outer_loop_count++;
    if (ctx->outer_loop_count >= ctx->outer_loop_div)
    {
        float vo_err = ctx->vo_ref - meas->vout;
        ctx->outer_loop_count = 0u;
        ctx->il_ref = pi_run(&ctx->vo_loop, vo_err, Ts * (float)ctx->outer_loop_div);
        if (ctx->il_ref < BUCKBOOST_FROM_MICRO(BUCKBOOST_IL_REF_MIN_MICRO)) { ctx->il_ref = BUCKBOOST_FROM_MICRO(BUCKBOOST_IL_REF_MIN_MICRO); }
        if (ctx->il_ref > BUCKBOOST_FROM_MICRO(BUCKBOOST_IL_REF_MAX_MICRO)) { ctx->il_ref = BUCKBOOST_FROM_MICRO(BUCKBOOST_IL_REF_MAX_MICRO); }
    }

    ctx->buck_mode = (ctx->vo_ref <= (meas->vin * BUCKBOOST_FROM_MICRO(BUCKBOOST_DUTY_MAX_MICRO)));
    {
        float il_err = ctx->il_ref - meas->il;
        float duty = pi_run(&ctx->il_loop, il_err, Ts);
        if (ctx->buck_mode)
        {
            ctx->duty_leg_a = duty;
            ctx->duty_leg_b = BUCKBOOST_FROM_MICRO(BUCKBOOST_DUTY_MAX_MICRO);
        }
        else
        {
            ctx->duty_leg_a = BUCKBOOST_FROM_MICRO(BUCKBOOST_DUTY_MAX_MICRO);
            ctx->duty_leg_b = duty;
        }
    }

    cmd.duty_leg_a = ctx->duty_leg_a;
    cmd.duty_leg_b = ctx->duty_leg_b;
    buckboost_hal_apply_pwm(hal, &cmd);
}

void buckboost_ctrl_set_vo_ref(buckboost_ctrl_ctx_t *ctx, float vo_ref)
{
    float vo_min = BUCKBOOST_FROM_MICRO(BUCKBOOST_VO_REF_MIN_MICRO);
    float vo_max = BUCKBOOST_FROM_MICRO(BUCKBOOST_VO_REF_MAX_MICRO);
    if (vo_ref < vo_min) { vo_ref = vo_min; }
    if (vo_ref > vo_max) { vo_ref = vo_max; }
    ctx->vo_ref = vo_ref;
}

static float pi_run(pi_ctrl_t *pi, float err, float Ts)
{
    float out;
    pi->integral += pi->ki * err * Ts;
    out = pi->kp * err + pi->integral;
    if (out > pi->out_max)
    {
        out = pi->out_max;
        pi->integral = out - pi->kp * err;
    }
    else if (out < pi->out_min)
    {
        out = pi->out_min;
        pi->integral = out - pi->kp * err;
    }
    return out;
}

/************************ (C) COPYRIGHT 2024 *****END OF FILE****/
