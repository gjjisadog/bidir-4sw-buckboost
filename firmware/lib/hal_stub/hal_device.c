/**
  ******************************************************************************
  * @file    hal_device.c
  * @brief   HAL device layer stub implementation for F28335.
  * @note    This is a minimal implementation. Replace with actual platform HAL.
  ******************************************************************************
  */

#include "hal_device.h"
#include <string.h>

static hal_device_t g_devices[8] = {0};
static int g_device_count = 0;

void HAL_device_Init(void)
{
    memset(g_devices, 0, sizeof(g_devices));
    g_device_count = 0;
}

hal_device_t HAL_device_OpenByName(const char *name, uint32_t flags)
{
    (void)flags;
    if (name == NULL || g_device_count >= 8)
    {
        return DEVICE_NULL;
    }
    /* Return a non-null pointer as device handle */
    g_devices[g_device_count] = (hal_device_t)(g_device_count + 1);
    g_device_count++;
    return g_devices[g_device_count - 1];
}

int HAL_device_Ioctl(hal_device_t dev, uint32_t cmd, void *param)
{
    (void)dev;
    (void)cmd;
    (void)param;
    /* Stub implementation - replace with actual platform code */
    return 0;
}

int HAL_device_Read(hal_device_t dev, void *buffer, uint32_t len)
{
    (void)dev;
    if (buffer != NULL && len > 0)
    {
        memset(buffer, 0, len);
    }
    return (int)len;
}

int HAL_device_Write(hal_device_t dev, const void *buffer, uint32_t len)
{
    (void)dev;
    (void)buffer;
    (void)len;
    return (int)len;
}


