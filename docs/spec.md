# Power Stage Specification (WIP)

- 输入电压范围：10–60 V（根据实际硬件可调整）
- 输出电压目标：示例 400 VDC（外环参考，可在软件中修改）
- 最大输出电流：根据硬件额定（占空限幅与 OCP 需匹配设计）
- 开关频率：20 kHz（F28335 ePWM1/2，对称双极性）
- 控制环路：电流内环 PI（20 kHz）、电压外环 PI（1 kHz 分频）
- 保护阈值：OVP/UVP/OCP/OTP 见 `firmware/bsp/buckboost_f28335_config.h`
- 采样通道：VIN、VOUT、IL、温度（ADC 同步采样）
- 软启动与状态机：上电 -> 预充 -> 软启动 -> 运行 -> 故障锁定
