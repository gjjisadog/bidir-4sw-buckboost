# 编译和烧录指南

本文档说明如何在不依赖CCS的情况下编译和烧录F28335固件。

## 已完成的工作

✅ **完整的构建系统**
- Makefile支持TI C2000 GCC工具链
- 自动检测和包含FreeRTOS
- 支持HAL stub实现

✅ **链接脚本和启动代码**
- F28335内存映射链接脚本
- 启动代码和中断向量表
- 系统初始化代码

✅ **烧录支持**
- 支持OpenOCD（如果支持F28335）
- 支持TI DSLite（推荐）
- 提供便捷的烧录脚本

✅ **HAL层实现**
- 创建了HAL stub实现（位于`firmware/lib/hal_stub/`）
- 这些是占位实现，需要替换为实际平台HAL

## 快速开始

### 1. 安装工具链

**macOS:**
```bash
# 从TI官网下载C2000 GCC工具链
# https://www.ti.com/tool/C2000WARE-GCC
# 解压后添加到PATH
export PATH=$PATH:/path/to/ti-cgt-c2000/bin
```

**Linux:**
```bash
# 类似地安装工具链
export PATH=$PATH:/path/to/ti-cgt-c2000/bin
```

### 2. 准备FreeRTOS

```bash
cd firmware/lib
# 从 https://www.freertos.org/ 下载FreeRTOS
# 解压后确保目录结构为:
# FreeRTOS/
#   Source/
#     include/
#     portable/
#       GCC/
#         C2000/
```

### 3. 编译项目

```bash
cd firmware/projects
make
```

如果一切正常，会在`build/bin/`目录生成：
- `buckboost_f28335.elf` - ELF格式（用于调试）
- `buckboost_f28335.hex` - Intel HEX格式（用于烧录）
- `buckboost_f28335.bin` - 二进制格式

### 4. 烧录到芯片

**方法1: 使用DSLite（推荐）**

```bash
# 设置环境变量
export DSLITE=/path/to/ccs_base/common/uscif/dslite
export CCXML=/path/to/F28335.ccxml

# 烧录
make flash-dslite
# 或使用脚本
./flash.sh
```

**方法2: 使用CCS手动烧录**
1. 打开CCS
2. 导入ELF文件或直接打开项目
3. 使用CCS的烧录功能

## 项目结构

```
firmware/
├── app/                    # 应用代码
│   ├── main.c
│   ├── buckboost_ctrl.c
│   └── ...
├── drivers/               # 驱动层
│   └── buckboost_hal.c
├── bsp/                   # 板级支持包
│   └── buckboost_f28335_config.h
├── lib/
│   ├── hal_stub/          # HAL stub实现（需替换）
│   │   ├── hal_device.c
│   │   ├── hal_bsp.c
│   │   └── ...
│   └── FreeRTOSConfig.h   # FreeRTOS配置
└── projects/              # 构建系统
    ├── Makefile
    ├── F28335.ld          # 链接脚本
    ├── startup.c          # 启动代码
    ├── flash.sh           # 烧录脚本
    └── BUILD.md           # 详细文档
```

## 重要说明

### HAL层实现

当前项目使用HAL stub实现（位于`firmware/lib/hal_stub/`）。这些是**占位实现**，仅用于编译通过。在实际使用前，需要：

1. **替换为实际平台HAL**
   - 根据README.md，项目依赖平台层HAL
   - 需要实现完整的设备驱动（PWM、ADC、GPIO等）

2. **集成平台层代码**
   - 根据README，需要平台层的驱动、HAL、OS等
   - 这些代码不在当前仓库中

### FreeRTOS

项目依赖FreeRTOS，Makefile会自动检测。如果未找到FreeRTOS，编译会失败。

### 链接脚本

`F28335.ld`是基于F28335标准内存映射的链接脚本。如果您的硬件有特殊配置，可能需要调整。

### 启动代码

`startup.c`包含基本的启动代码和中断向量表。可能需要根据实际需求：
- 完善系统初始化（PLL、时钟等）
- 添加更多中断处理函数
- 配置外设初始化

## 故障排除

### 编译错误：找不到工具链
```bash
# 检查工具链是否在PATH中
which c2000-elf-gcc

# 或设置TOOLCHAIN_PREFIX
export TOOLCHAIN_PREFIX=c2000-elf-
make
```

### 编译错误：找不到FreeRTOS
```bash
# 检查FreeRTOS目录
ls firmware/lib/FreeRTOS

# 或设置FREERTOS_DIR
export FREERTOS_DIR=/path/to/FreeRTOS
make
```

### 链接错误：未定义的符号
- 检查是否缺少库文件
- 检查HAL实现是否完整
- 查看链接脚本是否正确

### 烧录失败
- 检查调试器连接
- 检查CCXML配置
- 尝试使用CCS手动烧录一次验证硬件

## 下一步

1. ✅ 集成FreeRTOS源码
2. ⚠️ 替换HAL stub为实际平台HAL
3. ⚠️ 完善启动代码和中断处理
4. ⚠️ 添加单元测试
5. ⚠️ 优化链接脚本（根据实际硬件）

## 参考文档

- [BUILD.md](firmware/projects/BUILD.md) - 详细构建说明
- [README.md](README.md) - 项目总体说明
- [firmware/README.md](firmware/README.md) - 固件说明


