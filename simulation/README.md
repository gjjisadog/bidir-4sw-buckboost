# Simulation 目录说明

本目录包含Buck-Boost转换器的仿真和计算脚本。

## Python环境设置

### 方法1: 使用pip安装（推荐）

```bash
# 进入simulation目录
cd simulation

# 安装所有依赖
pip install -r requirements.txt

# 或者只安装核心库
pip install numpy matplotlib
```

### 方法2: 使用conda（如果使用Anaconda/Miniconda）

```bash
# 创建新环境（可选）
conda create -n buckboost python=3.9
conda activate buckboost

# 安装依赖
conda install numpy matplotlib jupyter

# 或者使用pip
pip install -r requirements.txt
```

### 方法3: 单独安装库

```bash
# 安装numpy
pip install numpy

# 安装matplotlib
pip install matplotlib

# 安装jupyter（如果需要运行notebook）
pip install jupyter
```

## 运行Jupyter Notebook

```bash
# 启动Jupyter Notebook
jupyter notebook

# 或者使用JupyterLab
jupyter lab
```

## 常见问题

### 问题1: pip命令找不到
- **macOS/Linux**: 使用 `pip3` 代替 `pip`
- **Windows**: 确保Python已添加到PATH

### 问题2: 权限错误
```bash
# 使用用户安装（推荐）
pip install --user -r requirements.txt
```

### 问题3: 虚拟环境（推荐）
```bash
# 创建虚拟环境
python -m venv venv

# 激活虚拟环境
# macOS/Linux:
source venv/bin/activate
# Windows:
venv\Scripts\activate

# 安装依赖
pip install -r requirements.txt
```

## 文件说明

- `calculations/Calculation_Report.ipynb` - 硬件参数计算书
- `requirements.txt` - Python依赖列表
- `simulink/` - Simulink仿真文件
- `plecs/` - PLECS仿真文件（预留）
- `spice/` - SPICE仿真文件（预留）

