/**
  ******************************************************************************
  * @file    buckboost_sfunc.c
  * @brief   Level-2 C S-function wrapping buck-boost control for simulation.
  ******************************************************************************
  */

#define S_FUNCTION_NAME  buckboost_sfunc
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"
#include <string.h>
#include <math.h>
#include "../../firmware/app/buckboost_ctrl.h"
#include "../../firmware/drivers/buckboost_hal.h"
#include "../../firmware/app/buckboost_fault.h"
#include "../../firmware/app/buckboost_state_machine.h"
#include "../../firmware/bsp/buckboost_f28335_config.h"
#include "hal_sim_stub.h"

static buckboost_hal_t g_hal;
static buckboost_ctrl_ctx_t g_ctrl;
static buckboost_fault_ctx_t g_fault;
static buckboost_state_ctx_t g_sm;
static buckboost_meas_t g_meas;

static uint16_t prv_to_raw(float value, float gain, float offset)
{
    float raw = (value - offset) / gain;
    if (raw < 0.0f) { raw = 0.0f; }
    if (raw > 65535.0f) { raw = 65535.0f; }
    return (uint16_t)(raw + 0.5f);
}

static void prv_feed_adc(float vin, float vout, float il, float temp)
{
    uint16_t vin_raw  = prv_to_raw(vin,  g_hal.adc_vin_gain,  g_hal.adc_vin_offset);
    uint16_t vout_raw = prv_to_raw(vout, g_hal.adc_vout_gain, g_hal.adc_vout_offset);
    uint16_t il_raw   = prv_to_raw(il,   g_hal.adc_il_gain,   g_hal.adc_il_offset);
    uint16_t t_raw    = prv_to_raw(temp, g_hal.adc_temp_gain, g_hal.adc_temp_offset);
    sim_hal_set_adc_raw(vin_raw, vout_raw, il_raw, t_raw);
}

static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, 0);
    if (!ssSetNumInputPorts(S, 1)) return;
    ssSetInputPortWidth(S, 0, 4); /* vin, vout, il, temp */
    ssSetInputPortDirectFeedThrough(S, 0, 1);

    if (!ssSetNumOutputPorts(S, 1)) return;
    ssSetOutputPortWidth(S, 0, 6); /* dutyA, dutyB, buckMode, gateEn, faultId, state */

    ssSetNumSampleTimes(S, 1);
    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 0);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);
}

static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, 1.0 / (double)BUCKBOOST_PWM_FREQUENCY_HZ);
    ssSetOffsetTime(S, 0, 0.0);
}

#define MDL_START
static void mdlStart(SimStruct *S)
{
    (void)S;
    memset(&g_hal, 0, sizeof(g_hal));
    memset(&g_ctrl, 0, sizeof(g_ctrl));
    memset(&g_fault, 0, sizeof(g_fault));
    memset(&g_sm, 0, sizeof(g_sm));
    memset(&g_meas, 0, sizeof(g_meas));

    buckboost_hal_init(&g_hal);
    buckboost_fault_init(&g_fault);
    buckboost_state_init(&g_sm);
    buckboost_ctrl_init(&g_ctrl);
}

static void mdlOutputs(SimStruct *S, int_T tid)
{
    (void)tid;
    InputRealPtrsType u = ssGetInputPortRealSignalPtrs(S, 0);
    float vin  = (float)(*u[0]);
    float vout = (float)(*u[1]);
    float il   = (float)(*u[2]);
    float temp = (float)(*u[3]);

    prv_feed_adc(vin, vout, il, temp);
    buckboost_ctrl_fast_isr(&g_ctrl, &g_hal, &g_meas, &g_fault, &g_sm);

    real_T *y = ssGetOutputPortRealSignal(S, 0);
    y[0] = (real_T)g_ctrl.duty_leg_a;
    y[1] = (real_T)g_ctrl.duty_leg_b;
    y[2] = (real_T)(g_ctrl.buck_mode ? 1.0 : 0.0);
    y[3] = (real_T)(sim_hal_gate_enabled());
    y[4] = (real_T)g_fault.last;
    y[5] = (real_T)g_sm.state;
}

static void mdlTerminate(SimStruct *S)
{
    (void)S;
}

#ifdef MATLAB_MEX_FILE
#include "simulink.c"
#else
#include "cg_sfun.h"
#endif

/************************ (C) COPYRIGHT 2024 *****END OF FILE****/
