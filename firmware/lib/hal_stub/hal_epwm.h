/**
  ******************************************************************************
  * @file    hal_epwm.h
  * @brief   EPWM HAL definitions for F28335.
  ******************************************************************************
  */

#ifndef __HAL_EPWM_H__
#define __HAL_EPWM_H__

#include <stdint.h>

typedef struct
{
    uint16_t uiCmpA;
    uint16_t uiCmpB;
} HAL_epwm_cmp;

#define EPWM_CTL_SET_EN_EPWM_CLK     0x0Du
#define EPWM_CTL_SET_FREQUECY        0x09u
#define EPWM_CTL_SET_DEATH_TIMER     0x0Au
#define EPWM_CTL_SET_APPINTERRUPT    0x0Bu
#define EPWM_CTL_SET_CMPAB           0x14u
#define EPWM_CTL_SET_DIS_EPMW_WAVE   0x16u
#define EPWM_CTL_SET_EN_EPMW_WAVE    0x17u
#define EPWM_CTL_SET_EN_TZ_INT_OST   0x22u

#endif /* __HAL_EPWM_H__ */


