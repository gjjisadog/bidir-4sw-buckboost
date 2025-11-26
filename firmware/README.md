# Firmware Layout

固件按照 APP/Drivers/BSP 分层，便于与平台层解耦并匹配 fsbb-converter 框架。

- `app/`：控制逻辑、状态机、故障处理与入口 `main.c`。`buckboost_ctrl_fast_isr` 由 ePWM1 中断驱动，FreeRTOS 在 `main.c` 中启动。
- `drivers/`：Buck-Boost 专用 HAL 粘合层，负责 PWM/ADC/GPIO/看门狗设备打开与命令下发。
- `bsp/`：F28335 板级配置与阈值 (`buckboost_f28335_config.h`)，以整数宏提供时钟、占空/PI 限幅、保护门限与 ADC 增益。
- `lib/`：第三方库或 DSP 库占位。
- `projects/`：CCS/IAR/VSCode 等 IDE 工程、链接脚本与启动文件放置处。

## 编译要点（CCS 示例）
1) 将 `app/*.c`、`drivers/*.c` 加入工程。
2) Include 路径增加：`firmware/app`, `firmware/drivers`, `firmware/bsp`，以及平台层 (`platform/driver/...`, `platform/hal`, `platform/os`, `platform/utils`)。
3) 放置并调整链接脚本/启动文件到 `firmware/projects/`；根据 F28335 Flash/RAM 区域与 FreeRTOS 配置修改。
4) 硬件映射或阈值通过 `bsp/buckboost_f28335_config.h` 修改；`buckboost_hal.c` 依赖设备名宏如 `DEVICE_NAME_EPWM1/ADCA`。

## 主要文件
- `app/main.c`：初始化 HAL/故障/状态机/控制，注册 ePWM ISR，启动 FreeRTOS 任务。
- `app/buckboost_ctrl.*`：电流/电压双 PI 控制、Buck/Boost 模式切换、PWM 占空生成。
- `app/buckboost_state_machine.*`：上电、预充、软启动、运行、故障锁定状态。
- `app/buckboost_fault.*`：软保护门限与故障锁存。
- `drivers/buckboost_hal.*`：PWM/ADC/GPIO/看门狗驱动适配、占空与死区下发、门极使能。

## 仿真
`simulation/simulink` 复用同一套头文件，提供 S-Function 与 HAL 仿真桩，可用 `build_mex.m` 构建。
