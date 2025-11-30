/**
  ******************************************************************************
  * @file    hal_gpio.h
  * @brief   GPIO HAL definitions for F28335.
  ******************************************************************************
  */

#ifndef __HAL_GPIO_H__
#define __HAL_GPIO_H__

#include <stdint.h>

typedef struct
{
    uint16_t uiGpioNum;
} hal_gpio_comm_cfg_type;

#define GPIO_CTL_SET_ON   0x01u
#define GPIO_CTL_SET_OFF  0x02u

#endif /* __HAL_GPIO_H__ */


