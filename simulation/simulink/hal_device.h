/**
  ******************************************************************************
  * @file    hal_device.h (simulation stub)
  * @brief   Minimal device layer definitions for MEX/Simulink build.
  ******************************************************************************
  */

#ifndef _HAL_DEVICE_H_
#define _HAL_DEVICE_H_

#include <stdint.h>

typedef void* hal_device_t;

#define DEVICE_NULL        ((hal_device_t)0)
#define DEVICE_FLAG_RDWR   (0u)

#define DEVICE_NAME_EPWM1       "epwm1"
#define DEVICE_NAME_EPWM2       "epwm2"
#define DEVICE_NAME_EPWM3       "epwm3"
#define DEVICE_NAME_EPWM4       "epwm4"
#define DEVICE_NAME_EPWM5       "epwm5"
#define DEVICE_NAME_EPWM6       "epwm6"
#define DEVICE_NAME_EPWM7       "epwm7"
#define DEVICE_NAME_EPWM8       "epwm8"
#define DEVICE_NAME_EPWM9       "epwm9"
#define DEVICE_NAME_EPWM10      "epwm10"
#define DEVICE_NAME_EPWM11      "epwm11"
#define DEVICE_NAME_EPWM12      "epwm12"
#define DEVICE_NAME_ADCA        "adca"
#define DEVICE_NAME_GPIO        "gpio"
#define DEVICE_NAME_BOARD_PROBE "board_probe"
#define DEVICE_NAME_TIMER0      "timer0"
#define DEVICE_NAME_WATCHDOG    "watchdog"

void HAL_device_Init(void);
hal_device_t HAL_device_OpenByName(const char *name, uint32_t flags);
int HAL_device_Ioctl(hal_device_t dev, uint32_t cmd, void *param);
int HAL_device_Read(hal_device_t dev, void *buffer, uint32_t len);
int HAL_device_Write(hal_device_t dev, const void *buffer, uint32_t len);

#endif /* _HAL_DEVICE_H_ */

/************************ (C) COPYRIGHT 2024 *****END OF FILE****/
