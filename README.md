# 四开关 Buck-Boost (F28335) 项目骨架

项目已按 fsbb-converter 框架拆分固件/硬件/仿真/工具目录，便于协同 TI F28335 平台、文档与硬件设计。

## 目录结构
- `firmware/app/`：控制逻辑、状态机、故障处理与入口 `main.c`。
- `firmware/drivers/`：Buck-Boost 专用 HAL 粘合层（当前 `buckboost_hal.*`）。
- `firmware/bsp/`：板级配置与常量（`buckboost_f28335_config.h`）。
- `firmware/lib/`：第三方库或 DSP 库占位。
- `firmware/projects/`：CCS/IAR/VSCode 等工程文件。
- `hardware/`：`schematic/`、`layout/`、`fabrication/`、`datasheet/`。
- `simulation/`：`simulink/` S-Function 与 HAL 仿真桩；预留 `plecs/`、`spice/`、`calculations/`。
- `docs/`：需求、设计笔记、测试报告、图片。
- `tools/`：脚本或上位机程序。
- `.gitignore`：忽略编译与仿真产物。

## VS Code 调试（无需打开 CCS GUI）
- 使用 VS Code + cpptools + TI 命令行工具：模板位于 `tools/vscode/`（`launch.json.example`、`tasks.json.example`、`F28335.ccxml.example`）。
- 依赖：TI C2000 GNU 工具链带的 `c2000-elf-gdb`（或等效 GDB）、TI DebugServer/DSLite CLI（ccs_base），以及匹配硬件的 CCXML。
- 将模板复制到 `.vscode/` 后，设置环境变量 `TI_C2000_GDB`、`TI_DSLITE`、`TI_CCXML`，并替换真实的构建命令与固件 `.out` 路径。
- 通过 VS Code 任务启动 DSLite GDB server，再运行 `F28335 (DSLite GDB server)` 启动配置即可在 VS Code 内断点/单步。

## 固件依赖与包含路径
- 编译器：CCS 10+（C2000）或等效，支持 F28335。
- 复用平台层：
  - 驱动：`platform/driver/01_ti_c2000/f28335/inc`、`.../src`
  - HAL：`platform/hal/inc`、`platform/hal/src`
  - OS：`platform/os/freertos`
  - 公共工具：`platform/utils`
- 建议在工程中加入的 include 路径：
  - `firmware/app`
  - `firmware/drivers`
  - `firmware/bsp`
  - 以上平台层路径
- 链接脚本/启动文件：放在 `firmware/projects/`，可参考储能项目脚本后按 F28335 地址空间调整。

## 控制与功能要点
- 20 kHz ePWM1/2 双半桥，硬件互锁与死区，ADC 触发对齐 TBCTR=ZRO。
- VIN/VOUT/IL/温度同步采样，增益与偏置通过 BSP 配置。
- 电流内环 PI（20 kHz）、电压外环 PI（1 kHz 分频），Buck/Boost 模式自动切换。
- 状态机：上电 -> 预充 -> 软启动 -> 运行 -> 故障锁定，带 ADC/OVP/UVP/OCP/OTP 检查。
- 安全：软启动斜坡、占空限幅、看门狗喂狗接口、硬件闭锁占位。

## 快速上手
1) 在 CCS 创建 C2000 工程，工作目录指向 `firmware/projects/`。
2) 将 `firmware/app/*.c`、`firmware/drivers/*.c` 加入工程，添加上述 include 路径。
3) 引入平台层源码/库与 FreeRTOS，放置并调整链接脚本/启动文件。
4) 根据硬件修改 `firmware/bsp/buckboost_f28335_config.h`（时钟、PWM/ADC 映射、增益、阈值）。
5) 编译并用 XDS100/110 或 JTAG 下载调试。

## 仿真
- `simulation/simulink` 提供 S-Function (`buckboost_sfunc.c`) 与 HAL 仿真桩，可用 `build_mex.m` 构建。
- 预留 `plecs/`、`spice/`、`calculations/` 目录以补充其他仿真与参数推导。

## 后续扩展
- 将参数与标定映射到现有通讯协议（bsw_serial_comm）便于上位机调试。
- 结合硬件设计完善数字 IO 与保护闭锁。
- 如需多处理器/协同外设，可参考储能项目 IPC/EMIF 结构。
