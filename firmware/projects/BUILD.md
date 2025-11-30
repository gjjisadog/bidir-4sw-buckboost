# 构建说明

本文档说明如何在不依赖CCS的情况下编译和烧录F28335固件。

## 前置要求

### 1. 安装TI C2000 GCC工具链

从TI官网下载并安装C2000 GCC工具链：
- 下载地址: https://www.ti.com/tool/C2000WARE-GCC
- 或者使用包管理器安装（如果可用）

安装后，确保工具链在PATH中，或者设置`TOOLCHAIN_PREFIX`环境变量。

### 2. 安装烧录工具（二选一）

#### 选项A: OpenOCD
```bash
# macOS
brew install openocd

# Linux
sudo apt-get install openocd

# 需要配置OpenOCD配置文件以支持F28335
```

#### 选项B: TI DSLite（推荐）
- 需要安装CCS（仅用于获取DSLite工具）
- DSLite位于: `ccs_base/common/uscif/dslite`
- 需要CCXML配置文件（参考`tools/vscode/F28335.ccxml.example`）

## 编译步骤

### 1. 进入项目目录
```bash
cd firmware/projects
```

### 2. 设置工具链（如果需要）
```bash
export TOOLCHAIN_PREFIX=c2000-elf-
```

### 3. 编译
```bash
make
```

编译完成后，生成的文件位于`build/bin/`目录：
- `buckboost_f28335.elf` - ELF格式（用于调试）
- `buckboost_f28335.hex` - Intel HEX格式（用于烧录）
- `buckboost_f28335.bin` - 二进制格式

### 4. 查看编译信息
```bash
make help
```

## 烧录步骤

### 方法1: 使用OpenOCD
```bash
make flash
```

注意：需要配置OpenOCD以支持F28335。如果OpenOCD不支持F28335，请使用方法2。

### 方法2: 使用TI DSLite（推荐）
```bash
# 设置环境变量
export DSLITE=/path/to/ccs_base/common/uscif/dslite
export CCXML=/path/to/F28335.ccxml

# 烧录（方法A：使用Makefile）
make flash-dslite

# 烧录（方法B：使用脚本）
./flash.sh
```

### 方法3: 手动使用CCS烧录
1. 打开CCS
2. 导入项目或直接打开ELF文件
3. 使用CCS的烧录功能

## 项目结构

```
firmware/
├── app/              # 应用代码
├── drivers/          # 驱动层
├── bsp/              # 板级支持包
├── lib/
│   └── hal_stub/     # HAL stub实现（需要替换为实际平台HAL）
└── projects/
    ├── Makefile      # 构建脚本
    ├── F28335.ld     # 链接脚本
    ├── startup.c     # 启动代码
    └── BUILD.md      # 本文档
```

## 注意事项

### HAL层实现
当前项目使用HAL stub实现（位于`firmware/lib/hal_stub/`），这些是占位实现，需要替换为实际的平台HAL层代码。

### FreeRTOS
项目依赖FreeRTOS。Makefile已支持自动检测FreeRTOS：

**选项1：使用项目内的FreeRTOS（推荐）**
```bash
# 下载FreeRTOS到 firmware/lib/FreeRTOS
cd firmware/lib
# 从 https://www.freertos.org/ 下载FreeRTOS
# 解压后确保目录结构为: firmware/lib/FreeRTOS/Source/...
```

**选项2：使用外部FreeRTOS路径**
```bash
export FREERTOS_DIR=/path/to/FreeRTOS
make
```

Makefile会自动检测FreeRTOS并包含相关源文件。如果未找到FreeRTOS，编译会失败（因为main.c使用了FreeRTOS API）。

**FreeRTOS配置**
- 配置文件位于: `firmware/lib/FreeRTOSConfig.h`
- 可根据实际需求调整配置参数

### 链接脚本
`F28335.ld`是基于F28335内存映射的链接脚本，可能需要根据实际硬件配置进行调整。

### 启动文件
`startup.c`包含基本的启动代码和中断向量表，可能需要根据实际需求完善。

## 故障排除

### 编译错误：找不到工具链
- 检查`TOOLCHAIN_PREFIX`是否正确
- 确保工具链在PATH中
- 尝试使用完整路径

### 链接错误：未定义的符号
- 检查是否缺少库文件（如FreeRTOS）
- 检查HAL层实现是否完整
- 查看链接脚本是否正确

### 烧录失败
- 检查调试器连接
- 检查CCXML配置是否正确
- 尝试使用CCS手动烧录一次以验证硬件连接

## 下一步

1. 集成FreeRTOS源码
2. 替换HAL stub为实际平台HAL
3. 完善启动代码和中断处理
4. 添加单元测试和集成测试

